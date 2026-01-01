#include <Windows.h>
#include <vfw.h>
#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <string>
#include <stdexcept>
#include <cstdlib>

// 链接 Windows 原生库
#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "ole32.lib")

// 显式声明宽字符版本的 AVIFile 函数
extern "C" {
HRESULT WINAPI AVIFileOpenW(PAVIFILE *ppfile, LPCWSTR szFile, UINT mode, LPCLSID pclsid);
}

// 全局配置（可调整）
std::atomic<bool> g_isRecording(false);
int g_screenWidth = GetSystemMetrics(SM_CXSCREEN);
int g_screenHeight = GetSystemMetrics(SM_CYSCREEN);
const int g_fps = 15;                // 录制帧率
const int g_bitCount = 24;           // 24位真彩色
const int g_compressQuality = 7500;  // MJPEG 压缩质量（0-10000）

// 安全释放位图
void SafeReleaseBitmap(HBITMAP& hBmp) {
    if (hBmp) {
        DeleteObject(hBmp);
        hBmp = nullptr;
    }
}

// 安全释放DC
void SafeReleaseDC(HDC& hDc) {
    if (hDc) {
        DeleteDC(hDc);
        hDc = nullptr;
    }
}

// 捕获单帧屏幕（无修改）
HBITMAP CaptureScreenFrame() {
    HDC hScreenDC = GetDC(NULL);
    if (!hScreenDC) {
        std::cerr << "错误：获取屏幕DC失败！" << std::endl;
        return nullptr;
    }

    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    if (!hMemDC) {
        ReleaseDC(NULL, hScreenDC);
        std::cerr << "错误：创建内存DC失败！" << std::endl;
        return nullptr;
    }

    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, g_screenWidth, g_screenHeight);
    if (!hBitmap) {
        SafeReleaseDC(hMemDC);
        ReleaseDC(NULL, hScreenDC);
        std::cerr << "错误：创建位图失败！" << std::endl;
        return nullptr;
    }

    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, g_screenWidth, g_screenHeight, hScreenDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBmp);

    SafeReleaseDC(hMemDC);
    ReleaseDC(NULL, hScreenDC);

    return hBitmap;
}

// 写入帧到AVI（保持无压缩数据获取逻辑）
BOOL WriteFrameToAVI(IAVIStream* pVideoStream, HBITMAP hBmp, int frameIndex) {
    if (!pVideoStream || !hBmp) {
        std::cerr << "错误：视频流或位图为空！" << std::endl;
        return FALSE;
    }

    // 1. 获取位图基础信息
    BITMAP bmpInfo = { 0 };
    if (!GetObject(hBmp, sizeof(BITMAP), &bmpInfo)) {
        std::cerr << "错误：获取位图信息失败！错误码：" << GetLastError() << std::endl;
        return FALSE;
    }

    // 2. 计算无压缩缓冲区大小
    DWORD dwBufferSize = bmpInfo.bmWidth * bmpInfo.bmHeight * (g_bitCount / 8);
    LPBYTE pFrameBuffer = (LPBYTE)malloc(dwBufferSize);
    if (!pFrameBuffer) {
        std::cerr << "错误：分配无压缩缓冲区失败！" << std::endl;
        return FALSE;
    }

    // 3. 配置无压缩的 BITMAPINFO（BI_RGB），biHeight 为负数保证方向
    BITMAPINFO bmi = { 0 };
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bmpInfo.bmWidth;
    bmi.bmiHeader.biHeight = -bmpInfo.bmHeight; // 顶行优先，核心：保持负数
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = g_bitCount;
    bmi.bmiHeader.biCompression = BI_RGB; // 无压缩
    bmi.bmiHeader.biSizeImage = dwBufferSize;

    // 4. 获取无压缩的 DIB 数据
    HDC hMemDC = CreateCompatibleDC(NULL);
    if (!hMemDC) {
        free(pFrameBuffer);
        std::cerr << "错误：创建内存DC失败！错误码：" << GetLastError() << std::endl;
        return FALSE;
    }

    HBITMAP hOldBmp = (HBITMAP)SelectObject(hMemDC, hBmp);
    BOOL bSuccess = GetDIBits(hMemDC, hBmp, 0, bmpInfo.bmHeight,
                              pFrameBuffer, &bmi, DIB_RGB_COLORS);
    SelectObject(hMemDC, hOldBmp);
    DeleteDC(hMemDC);

    if (!bSuccess) {
        free(pFrameBuffer);
        std::cerr << "错误：获取DIB数据失败！错误码：" << GetLastError() << std::endl;
        return FALSE;
    }

    // 5. 写入无压缩数据，由编码器自动压缩为 MJPEG
    HRESULT hr = AVIStreamWrite(pVideoStream,
                                frameIndex,    // 帧索引
                                1,             // 写入帧数
                                pFrameBuffer,  // 无压缩数据缓冲区
                                dwBufferSize,  // 无压缩缓冲区大小
                                AVIIF_KEYFRAME,// 关键帧
                                NULL, NULL);

    // 6. 释放缓冲区
    free(pFrameBuffer);

    if (hr != AVIERR_OK) {
        std::cerr << "错误：写入帧失败！AVI错误码：" << hr << std::endl;
        return FALSE;
    }

    return TRUE;
}

// 录屏核心函数（修复画面方向）
void StartScreenRecording(const std::wstring& outputPath) {
    AVIFileInit(); // 初始化 AVIFile 库

    IAVIFile* pAviFile = nullptr;
    IAVIStream* pVideoStream = nullptr;
    AVISTREAMINFO streamInfo = { 0 };
    int frameIndex = 0;

    try {
        // 1. 创建 AVI 文件（宽字符版本）
        HRESULT hr = AVIFileOpenW(&pAviFile,
                                 outputPath.c_str(),
                                 OF_WRITE | OF_CREATE | OF_SHARE_DENY_WRITE,
                                 NULL);
        if (hr != AVIERR_OK) {
            throw std::runtime_error("创建AVI文件失败！错误码：" + std::to_string(hr));
        }

        // 2. 配置视频流（修复：rcFrame 高度为负数）
        streamInfo.fccType = streamtypeVIDEO;
        streamInfo.fccHandler = mmioFOURCC('M', 'J', 'P', 'G'); // MJPEG 编码器
        streamInfo.dwScale = 1;
        streamInfo.dwRate = g_fps;                             // 帧率
        streamInfo.dwQuality = g_compressQuality;              // 压缩质量（0-10000）
        streamInfo.dwSuggestedBufferSize = g_screenWidth * g_screenHeight * 3; // 无压缩缓冲区大小
        SetRect(&streamInfo.rcFrame, 0, 0, g_screenWidth, -g_screenHeight); // 关键：高度为负数

        // 3. 创建 MJPEG 压缩的视频流
        hr = AVIFileCreateStream(pAviFile, &pVideoStream, &streamInfo);
        if (hr != AVIERR_OK) {
            throw std::runtime_error("创建视频流失败！错误码：" + std::to_string(hr));
        }

        // 4. 设置视频流格式（修复：biHeight 为负数）
        BITMAPINFOHEADER bmpHeader = { 0 };
        bmpHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpHeader.biWidth = g_screenWidth;
        bmpHeader.biHeight = -g_screenHeight; // 关键：改回负数（顶行优先）
        bmpHeader.biPlanes = 1;
        bmpHeader.biBitCount = g_bitCount;
        bmpHeader.biCompression = BI_RGB; // 输入为无压缩格式
        bmpHeader.biSizeImage = g_screenWidth * g_screenHeight * 3;

        hr = AVIStreamSetFormat(pVideoStream, 0, &bmpHeader, sizeof(BITMAPINFOHEADER));
        if (hr != AVIERR_OK) {
            throw std::runtime_error("设置视频格式失败！错误码：" + std::to_string(hr));
        }

        // 输出录屏信息
        char pathBuf[MAX_PATH] = { 0 };
        WideCharToMultiByte(CP_UTF8, 0, outputPath.c_str(), -1, pathBuf, MAX_PATH, NULL, NULL);
        std::cout << "========================================" << std::endl;
        std::cout << "开始录屏（MJPEG 压缩）！" << std::endl;
        std::cout << "保存路径：" << pathBuf << std::endl;
        std::cout << "分辨率：" << g_screenWidth << "x" << g_screenHeight << std::endl;
        std::cout << "帧率：" << g_fps << " FPS | 压缩质量：" << g_compressQuality / 100 << "%" << std::endl;
        std::cout << "按 Ctrl+C 停止录屏..." << std::endl;
        std::cout << "========================================" << std::endl;

        g_isRecording = true;
        while (g_isRecording) {
            auto frameStart = std::chrono::high_resolution_clock::now();

            // 捕获并写入帧
            HBITMAP hFrameBmp = CaptureScreenFrame();
            if (hFrameBmp) {
                if (WriteFrameToAVI(pVideoStream, hFrameBmp, frameIndex)) {
                    frameIndex++;
                } else {
                    std::cerr << "警告：第 " << frameIndex << " 帧写入失败，跳过！" << std::endl;
                    frameIndex++;
                }
                SafeReleaseBitmap(hFrameBmp);
            } else {
                std::cerr << "警告：第 " << frameIndex << " 帧捕获失败，跳过！" << std::endl;
                frameIndex++;
            }

            // 控制帧率
            auto frameEnd = std::chrono::high_resolution_clock::now();
            double elapsed = std::chrono::duration<double>(frameEnd - frameStart).count();
            double sleepTime = (1.0 / g_fps) - elapsed;
            if (sleepTime > 0) {
                std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "录屏异常：" << e.what() << std::endl;
        g_isRecording = false;
    }

    // 释放资源
    if (pVideoStream) AVIStreamRelease(pVideoStream);
    if (pAviFile) AVIFileRelease(pAviFile);
    AVIFileExit();

    std::cout << "========================================" << std::endl;
    std::cout << "录屏结束！共录制 " << frameIndex << " 帧" << std::endl;
    std::cout << "========================================" << std::endl;
}

// 控制台退出信号处理
BOOL CtrlHandler(DWORD fdwCtrlType) {
    if (fdwCtrlType == CTRL_C_EVENT) {
        g_isRecording = false;
        std::cout << "\n检测到 Ctrl+C，正在停止录屏..." << std::endl;
        return TRUE;
    }
    return FALSE;
}

int main() {
    // 设置控制台 UTF-8 编码（解决中文乱码）
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 注册 Ctrl+C 信号处理
    if (!SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE)) {
        std::cerr << "警告：设置控制台信号处理失败！错误码：" << GetLastError() << std::endl;
    }

    // 生成带时间戳的输出文件名
    SYSTEMTIME st;
    GetLocalTime(&st);
    wchar_t outputPath[MAX_PATH] = { 0 };
    swprintf_s(outputPath, MAX_PATH,
               L"ScreenRecord_%04d%02d%02d_%02d%02d%02d.avi",
               st.wYear, st.wMonth, st.wDay,
               st.wHour, st.wMinute, st.wSecond);

    try {
        StartScreenRecording(outputPath);
    } catch (const std::exception& e) {
        std::cerr << "程序异常：" << e.what() << std::endl;
    }

    std::cout << "按任意键退出..." << std::endl;
    system("pause > nul");
    return 0;
}
