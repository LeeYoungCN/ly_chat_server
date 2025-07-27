/**
 * @file log.h
 * @author LiYang
 * @brief 调试日志，不依赖Logger。
 * @version 0.1
 * @date 2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef COMMON_DEBUG_DEBUG_LOG_H
#define COMMON_DEBUG_DEBUG_LOG_H

#include <chrono>
#include <filesystem>
#include <format>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "common/types/log_level.h"

namespace common {
template <common::types::LogLevel level, typename... Args>
void CommonDebugLog(const char* file, int line, const char* func, std::format_string<Args...> fmt, Args&&... args)
{
    static std::mutex mtx;

    std::string message = std::format(fmt, std::forward<Args>(args)...);
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

    std::unique_lock<std::mutex> lock(mtx);

    std::cout << std::format("[{}] [{}] [Tid: {}] [debug_log] [{}:{} {}] {}",
                             timeSs.str(),
                             common::types::logLevelToStr(level),
                             std::hash<std::thread::id>()(std::this_thread::get_id()),
                             std::filesystem::path(file).filename().string(),
                             line,
                             func,
                             message)
              << std::endl;
}

}  // namespace common

#ifdef NDEBUG
   // Release模式：空操作，显式消费所有参数避免警告
#define COMMON_LOG(level, fmt, ...)             \
    do {                                        \
        (void)#level;       /* 处理level参数 */  \
        (void)fmt;          /* 单独处理格式串 */ \
        (void)(##__VA_ARGS__);/* 处理可变参数 */   \
    } while (0)
#else
   // Debug模式：实际日志输出，支持所有级别
#define COMMON_LOG(level, fmt, ...)                                   \
    do {                                                              \
        /* 传递级别、位置信息和日志内容 */                            \
        common::CommonDebugLog<common::types::LogLevel::level>(       \
            __FILE__,                      /* 文件名 */               \
            __LINE__,                      /* 行号 */                 \
            __func__,                      /* 函数名 */               \
            fmt __VA_OPT__(, ) __VA_ARGS__ /* 格式串+参数（C++17） */ \
        );                                                            \
    } while (0)
#endif


#ifdef NDEBUG

#define COMMON_LOG_INFO(fmt, ...) (static_cast<void>(0))

#define COMMON_LOG_WARN(fmt, ...) (static_cast<void>(0))

#define COMMON_LOG_ERR(fmt, ...) (static_cast<void>(0))

#define COMMON_LOG_FATAL(fmt, ...) (static_cast<void>(0))

#define COMMON_LOG_COND(condition, fmt, ...) (static_cast<void>(0))

#else

#define COMMON_LOG_INFO(fmt, ...) \
    common::CommonDebugLog<common::types::LogLevel::INFO>(__FILE__, __LINE__, __func__, fmt __VA_OPT__(, ) __VA_ARGS__);

#define COMMON_LOG_WARN(fmt, ...) \
    common::CommonDebugLog<common::types::LogLevel::WARN>(__FILE__, __LINE__, __func__, fmt __VA_OPT__(, ) __VA_ARGS__);

#define COMMON_LOG_ERR(fmt, ...) \
    common::CommonDebugLog<common::types::LogLevel::ERR>(__FILE__, __LINE__, __func__, fmt __VA_OPT__(, ) __VA_ARGS__);

#define COMMON_LOG_FATAL(fmt, ...)                                         \
    do {                                                                   \
        common::CommonDebugLog<common::types::LogLevel::FATAL>(            \
            __FILE__, __LINE__, __func__, fmt __VA_OPT__(, ) __VA_ARGS__); \
        std::abort();                                                      \
    } while (0)

#define COMMON_LOG_COND(condition, fmt, ...)                             \
    do {                                                                 \
        if (!(condition)) {                                              \
            COMMON_LOG_INFO("[Success]" fmt __VA_OPT__(, ) __VA_ARGS__); \
        }                                                                \
    } while (0)

#endif  // NDEBUG
#endif  // COMMON_DEBUG_DEBUG_LOG_H
