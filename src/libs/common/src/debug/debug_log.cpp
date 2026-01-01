#include "common/debug/debug_log.h"

#include "common/compiler/macros.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <sys/syscall.h>
#include <unistd.h>
#elif PLATFORM_MACOS
#include <pthread.h>
#else
#error "Unsupport system"
#endif

#include <chrono>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <ctime>
#include <filesystem>
#include <format>
#include <iostream>
#include <mutex>

#include "common/types/logging_types.h"

namespace {
LogLevel g_logLevel = LOG_LVL_INFO;

size_t GetCurrentThreadIdInternal()
{
#if PLATFORM_WINDOWS
    return static_cast<size_t>(GetCurrentThreadId());
#elif PLATFORM_LINUX
    return static_cast<size_t>(syscall(SYS_gettid));
#elif PLATFORM_MACOS
    uint64_t tid;
    pthread_threadid_np(nullptr, &tid);
    return static_cast<size_t>(tid);
#else
    return 0;
#endif
}

std::string TimeString()
{
    const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltm{};
#if PLATFORM_WINDOWS
    // Windows 使用 localtime_s
    localtime_s(&ltm, &now);
#else
    // Linux/macOS 使用 localtime_r
    localtime_r(&now, &ltm);
#endif
    std::stringstream timeSs;

    timeSs << std::put_time(&ltm, "%Y-%m-%d %H:%M:%S");

    return timeSs.str();
}

std::string formatLog(LogLevel level, const char* file, int line, const char* func, const std::string& message)
{
    return std::format("[{}] [{}] [Tid: {:#x}] [{}:{}] [{}] {}",
                       TimeString(),
                       logLevelToStr(level),
                       GetCurrentThreadIdInternal(),
                       std::filesystem::path(file).filename().string(),
                       line,
                       func,
                       message);
}
}  // namespace

extern "C" {
void CommonDebugLog(LogLevel level, const char* file, int line, const char* func, const char* fmt, ...)
{
    if (level < g_logLevel) {
        return;
    }
    static std::mutex mtx;
    std::unique_lock<std::mutex> lock(mtx);

    constexpr uint32_t BUFFER_LEN = 512;
    char buffer[BUFFER_LEN];
    va_list argList;
    va_start(argList, fmt);
    vsnprintf(buffer, BUFFER_LEN, fmt, argList);
    va_end(argList);

    std::string logStr = formatLog(level, file, line, func, buffer);
    std::cout << logStr << std::endl;
}

void SetDebugLogLevel(LogLevel level)
{
    g_logLevel = level;
}
}
