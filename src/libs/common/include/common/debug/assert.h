/**
 * @file assert.h
 * @author LiYang
 * @brief 公共模块，断言
 * @version 0.1
 * @date 2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef DEBUG_ASSERT_H
#define DEBUG_ASSERT_H

#include "common/debug/log.h"

#ifdef NDEBUG

#ifndef COMMON_ASSERT
#define COMMON_ASSERT(condition) (static_cast<void>(0))
#endif  // COMMON_ASSERT

#ifndef COMMON_ASSERT_MSG
#define COMMON_ASSERT_MSG(condition, msg, ...) (static_cast<void>(0))
#endif  // COMMON_ASSERT_MSG

#else

#ifndef COMMON_ASSERT
#define COMMON_ASSERT(condition)             \
    do {                                     \
        if (!(condition)) {                  \
            COMMON_LOG_ERR("ASSERT FAILED"); \
            std::abort();                    \
        }                                    \
    } while (0)
#endif  // COMMON_ASSERT

#ifndef COMMON_ASSERT_MSG
#define COMMON_ASSERT_MSG(condition, fmt, ...)              \
    do {                                                    \
        if (!(condition)) {                                 \
            COMMON_LOG_ERR(fmt __VA_OPT__(, ) __VA_ARGS__); \
            std::abort();                                   \
        }                                                   \
    } while (0)
#endif  // COMMON_ASSERT_MSG
#endif  // NDEBUG
#endif  // DEBUG_ASSERT_H
