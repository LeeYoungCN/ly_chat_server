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

#include "common/types/logging_types.h"

namespace common {
void CommonDebugLog(common::types::logging::LogLevel level, const char* file, int line, const char* func,
                    const char* fmt, ...);

}  // namespace common

#ifdef NDEBUG
   // Release模式：空操作，显式消费所有参数避免警告
#define COMMON_LOG(level, fmt, ...) (static_cast<void>(0))
#else
   // Debug模式：实际日志输出，支持所有级别
#define COMMON_LOG(level, fmt, ...)                                                                  \
    do {                                                                                             \
        common::CommonDebugLog(level, __FILE__, __LINE__, __func__, fmt __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)
#endif

#define COMMON_LOG_INFO(fmt, ...) COMMON_LOG(common::types::logging::LogLevel::INFO, fmt __VA_OPT__(, ) __VA_ARGS__);

#define COMMON_LOG_WARN(fmt, ...) COMMON_LOG(common::types::logging::LogLevel::WARN, fmt __VA_OPT__(, ) __VA_ARGS__);

#define COMMON_LOG_ERR(fmt, ...) COMMON_LOG(common::types::logging::LogLevel::ERR, fmt __VA_OPT__(, ) __VA_ARGS__);

#define COMMON_LOG_FATAL(fmt, ...)                                                           \
    do {                                                                                     \
        COMMON_LOG(common::types::logging::LogLevel::FATAL, fmt __VA_OPT__(, ) __VA_ARGS__); \
        std::abort();                                                                        \
    } while (0)

#define COMMON_LOG_COND(condition, fmt, ...)                             \
    do {                                                                 \
        if (!(condition)) {                                              \
            COMMON_LOG_ERR("Failed to " fmt __VA_OPT__(, ) __VA_ARGS__); \
        }                                                                \
    } while (0)
#define COMMON_LOG_EXCEPTION(e, fmt, ...)                             \
    do {                                                              \
        COMMON_LOG_ERR(fmt " excp: %s" __VA_OPT__(, ) __VA_ARGS__, e.what()); \
    } while (0)
#endif  // COMMON_DEBUG_DEBUG_LOG_H
