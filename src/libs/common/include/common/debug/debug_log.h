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

#include "common/debug/debug_level.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void set_debug_log_level(DebugLevel level);
void common_debug_log(const char* file, int line, const char* func, DebugLevel level, const char* format, ...);

#ifdef __cplusplus
}
#endif  // __cplusplus

#if defined(NDEBUG) && !defined(ENABLE_TEST)
// Release模式：空操作，显式消费所有参数避免警告
#define DEBUG_LOG(level, fmt, ...) (static_cast<void>(0))
#else
// Debug模式：实际日志输出，支持所有级别
#define DEBUG_LOG(level, fmt, ...)                                                                   \
    do {                                                                                             \
        common_debug_log(__FILE__, __LINE__, __FUNCTION__, level, fmt __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)
#endif

#define DEBUG_LOG_DBG(fmt, ...) DEBUG_LOG(DEBUG_LVL_DEBUG, fmt __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOG_INFO(fmt, ...) DEBUG_LOG(DEBUG_LVL_INFO, fmt __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOG_WARN(fmt, ...) DEBUG_LOG(DEBUG_LVL_WARN, fmt __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOG_ERR(fmt, ...) DEBUG_LOG(DEBUG_LVL_ERR, fmt __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOG_FATAL(fmt, ...)                                   \
    do {                                                            \
        DEBUG_LOG(DEBUG_LVL_FATAL, fmt __VA_OPT__(, ) __VA_ARGS__); \
        std::abort();                                               \
    } while (0)

#define DEBUG_LOG_COND(condition, fmt, ...)                            \
    do {                                                               \
        if (!(condition)) {                                            \
            DEBUG_LOG_ERR("[FAILED] " fmt __VA_OPT__(, ) __VA_ARGS__); \
        } else {                                                       \
            DEBUG_LOG_DBG("[SUCCESS] " fmt __VA_OPT__(, ) __VA_ARGS__) \
        }                                                              \
    } while (0)

#ifdef __cplusplus
#if defined(NDEBUG) && !defined(ENABLE_TEST)
#define DEBUG_LOG_EXCEPTION(e, fmt, ...) \
    do {                                 \
        static_cast<void>(e);            \
    } while (0)
#else  // defined(NDEBUG) && !defined(ENABLE_TEST)
#define DEBUG_LOG_EXCEPTION(e, fmt, ...)                                     \
    do {                                                                     \
        DEBUG_LOG_ERR(fmt " excp: %s" __VA_OPT__(, ) __VA_ARGS__, e.what()); \
    } while (0)
#endif  // defined(NDEBUG) && !defined(ENABLE_TEST)
#endif  // __cplusplus

#endif  // COMMON_DEBUG_DEBUG_LOG_H
