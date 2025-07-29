#include "common/debug/debug_log.h"

#include <chrono>
#include <cstdarg>
#include <cstdio>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <thread>

#include "common/types/logging_types.h"

namespace common {

using namespace common::types::logging;

std::string formatLog(common::types::logging::LogLevel level, const char* file, int line, const char* func,
                      const std::string& message)
{
    const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltm{};
#ifdef _WIN32
    // Windows 使用 localtime_s
    localtime_s(&ltm, &now);
#else
    // Linux/macOS 使用 localtime_r
    localtime_r(&now, &ltm);
#endif
    std::stringstream timeSs;

    timeSs << std::put_time(&ltm, "%Y-%m-%d %H:%M:%S");
    std::stringstream logStream;
    logStream << "[" << timeSs.str() << "] "
              << "[" << logLevelToStr(level) << "] "
              << "[Tid: " << std::this_thread::get_id() << "] " << "["
              << std::filesystem::path(file).filename().string() << ":" << line << "] " << "[" << func << "] "
              << message;
    return logStream.str();
}

void CommonDebugLog(common::types::logging::LogLevel level, const char* file, int line, const char* func,
                    const char* fmt, ...)
{
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
}  // namespace common
