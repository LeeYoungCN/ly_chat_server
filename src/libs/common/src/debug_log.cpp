#include <chrono>
#include <cstdarg>
#include <cstddef>
#include <ctime>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "common/debug/log.h"
#include "common/types/log_level.h"

// namespace common {
// std::string GetTimeStr(const std::string &fmt)
// {
//     const std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
//     std::tm ltm{};
// #ifdef _WIN32
//     // Windows 使用 localtime_s
//     localtime_s(&ltm, &now);
// #else
//     // Linux/macOS 使用 localtime_r
//     localtime_r(&now, &ltm);
// #endif
//     std::stringstream ss;
//     ss << std::put_time(&ltm, fmt.c_str());
//     return ss.str();
// }

// size_t GetPid()
// {
//     return std::hash<std::thread::id>()(std::this_thread::get_id());
// }

// template <common::types::LogLevel level, typename... Args>
// void CommonDebugLog(const char *file, int line, const std::string& fmt, Args&&... args)
// {
//     static std::mutex mtx;

//     // std::string message = std::format(fmt, std::forward<Args>(args)...);
//     std::unique_lock<std::mutex> lock(mtx);

//     std::cout << std::format("[{}] [{}] [Tid: {}] [{}:{}]",
//                              GetTimeStr("%Y-%m-%d %H:%M:%S"),
//                              common::types::logLevelToStr(level),
//                              GetPid(),
//                              std::filesystem::path(file).filename().string(),
//                              line,
//                              std::format(fmt, std::forward<Args>(args)...))
//               << std::endl;
// }
// }  // namespace common
