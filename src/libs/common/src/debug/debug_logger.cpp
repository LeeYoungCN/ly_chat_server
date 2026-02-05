#include "common/debug/debug_logger.h"
#include <mutex>
#include <system_error>

#include "common/compiler/macros.h"
#include "common/debug/debug_level.h"

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

namespace common::debug {
void DebugLogger::set_debug_log_level(DebugLevel level)
{
    _logLevel = level;
}

void DebugLogger::log_va(const char* file, int line, const char* func, DebugLevel level, const char* format,
                         va_list args)
{
    log_it(level, va_list_to_string(format, args), file, line, func);
}

void DebugLogger::log_it(DebugLevel level, const std::string& message, const char* file, int line, const char* func)
{
    if (!should_log(level)) {
        return;
    }
    auto logmsg = format_log(level, message, file, line, func);
    std::lock_guard lock(_mtx);
    std::cout << logmsg << std::endl;
}

bool DebugLogger::should_log(DebugLevel level)
{
    return (_logLevel != DebugLevel::DEBUG_LVL_OFF && level >= _logLevel);
}

std::string DebugLogger::format_log(DebugLevel level, const std::string& message, const char* file, int line,
                                    const char* func)
{
    if (file != nullptr) {
        return std::format("[{}][{}][Tid: {:#x}][{}:{}][{}] {}",
                           time_string(),
                           get_debug_log_lvl_str(level),
                           get_current_tid(),
                           std::filesystem::path(file).filename().string(),
                           line,
                           func,
                           message);
    } else {
        return std::format(
            "[{}] [{}] [Tid: {:#x}]: {}", time_string(), get_debug_log_lvl_str(level), get_current_tid(), message);
    }
}

size_t DebugLogger::get_current_tid()
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

std::string DebugLogger::time_string()
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

std::string DebugLogger::va_list_to_string(const char* format, va_list args)
{
    va_list argsCopy;

    va_copy(argsCopy, args);
    int len = vsnprintf(nullptr, 0, format, argsCopy);
    va_end(argsCopy);

    if (len < 0) {
        throw std::runtime_error("vsnprintf failed.");
    }

    std::string message(static_cast<uint32_t>(len), '\0');
    vsnprintf(message.data(), static_cast<uint32_t>(len) + 1, format, args);
    return message;
}
}  // namespace common::debug
