#include "common/debug/working_env.h"
#include <cstdint>

#include "common/compiler/macros.h"
#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <unistd.h>  // Linux的readlink函数
#elif PLATFORM_MACOS
#include <mach-o/dyld.h>  // macOS的_NSGetExecutablePath
#endif

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace {
std::string GetOsName()
{
#ifdef _WIN32
// Windows 32位或64位（_WIN32在64位系统上也会定义）
#ifdef _WIN64
    return "Windows 64-bit";
#else
    return "Windows 32-bit";
#endif
#elif defined(__linux__)
    // Linux 系统
    return "Linux";
#elif defined(__APPLE__) && defined(__MACH__)
    // macOS 系统（基于 Mach 内核）
    return "macOS";
#elif defined(__FreeBSD__)
    // FreeBSD 系统
    return "FreeBSD";
#elif defined(__NetBSD__)
    // NetBSD 系统
    return "NetBSD";
#elif defined(__OpenBSD__)
    // OpenBSD 系统
    return "OpenBSD";
#else
    // 未知操作系统
    return "Unknown OS";
#endif
}

std::vector<std::string> GetCompilers()
{
    static std::vector<std::string> compilers = {
#ifdef _MSC_VER
        "_MSC_VER",
#endif
#ifdef __clang__
        "__clang__",
#endif
#ifdef __MINGW32__
        "__MINGW32__",
#endif
#ifdef __MINGW64__
        "__MINGW64__",
#endif
#ifdef __GNUC__
        "__GNUC__",
#endif
    };
    return compilers;
}

std::string GetProcessPath()
{
    constexpr uint32_t BUFFER_LEN = 256;
    char path[BUFFER_LEN] = {'\0'};
#if PLATFORM_WINDOWS
    GetModuleFileNameA(nullptr, path, BUFFER_LEN);
#elif PLATFORM_LINUX
    auto length = readlink("/proc/self/exe", path, BUFFER_LEN - 1);
    path[length] = '\0';
#elif PLATFORM_MACOS
    uint32_t size = sizeof(path);
    _NSGetExecutablePath(path, &size)
#else
#error "Unsupport system."
#endif
    return path;
}

std::string GetArchitecture() {
    // x86 架构 (32位)
    #if defined(__i386__) || defined(_M_IX86)
        return "x86 (32-bit)";

    // x86_64 架构 (64位)
    #elif defined(__x86_64__) || defined(_M_X64)
        return "x86_64 (64-bit)";

    // ARM 架构 (32位)
    #elif defined(__arm__) || defined(_M_ARM)
        return "ARM (32-bit)";

    // ARM64 架构 (64位)
    #elif defined(__aarch64__) || defined(_M_ARM64)
        return "ARM64 (64-bit)";

    // 其他常见架构
    #elif defined(__powerpc__) || defined(__ppc__)
        return "PowerPC";
    #elif defined(__mips__)
        return "MIPS";
    #elif defined(__riscv)
        return "RISC-V";

    // 未知架构
    #else
        return "Unknown architecture";
    #endif
}

}  // namespace

namespace common::debug::working_env {
void ShowWorkingEnv()
{
    std::cout << "Os: " << GetOsName() << std::endl;
    std::cout << "Arch: " << GetArchitecture() << std::endl;
    std::cout << "Compilers: ";
    for (const auto& c : GetCompilers()) {
        std::cout << c << " ";
    }
    std::cout << std::endl;
    std::cout << "Wording directory: " << std::filesystem::current_path() << std::endl;
    std::cout << "Process: " << GetProcessPath() << std::endl;
}

}  // namespace common::debug::working_env
