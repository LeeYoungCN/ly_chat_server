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
void SetDebugLogLevel(common::types::logging::LogLevel level);

void CommonDebugLog(common::types::logging::LogLevel level, const char* file, int line, const char* func,
                    const char* fmt, ...);

}  // namespace common

#ifdef NDEBUG
   // Release模式：空操作，显式消费所有参数避免警告
#define DEBUG_LOG(level, fmt, ...) (static_cast<void>(0))
#else
   // Debug模式：实际日志输出，支持所有级别
#define DEBUG_LOG(level, fmt, ...)                                                                   \
    do {                                                                                             \
        common::CommonDebugLog(level, __FILE__, __LINE__, __func__, fmt __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)
#endif

#define DEBUG_LOG_DBG(fmt, ...) DEBUG_LOG(common::types::logging::LogLevel::DEBUG, fmt __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOG_INFO(fmt, ...) DEBUG_LOG(common::types::logging::LogLevel::INFO, fmt __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOG_WARN(fmt, ...) DEBUG_LOG(common::types::logging::LogLevel::WARN, fmt __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOG_ERR(fmt, ...) DEBUG_LOG(common::types::logging::LogLevel::ERR, fmt __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOG_FATAL(fmt, ...)                                                           \
    do {                                                                                    \
        DEBUG_LOG(common::types::logging::LogLevel::FATAL, fmt __VA_OPT__(, ) __VA_ARGS__); \
        std::abort();                                                                       \
    } while (0)

#define DEBUG_LOG_COND(condition, fmt, ...)                             \
    do {                                                                \
        if (!(condition)) {                                             \
            DEBUG_LOG_ERR(fmt " failed." __VA_OPT__(, ) __VA_ARGS__);   \
        } else {                                                        \
            DEBUG_LOG_DBG(fmt " successed." __VA_OPT__(, ) __VA_ARGS__) \
        }                                                               \
    } while (0)

#ifdef NDEBUG
#define DEBUG_LOG_EXCEPTION(e, fmt, ...) \
    do {                                 \
        static_cast<void>(e);            \
    } while (0)
#else
#define DEBUG_LOG_EXCEPTION(e, fmt, ...)                                     \
    do {                                                                     \
        DEBUG_LOG_ERR(fmt " excp: %s" __VA_OPT__(, ) __VA_ARGS__, e.what()); \
    } while (0)
#endif
#endif  // COMMON_DEBUG_DEBUG_LOG_H
