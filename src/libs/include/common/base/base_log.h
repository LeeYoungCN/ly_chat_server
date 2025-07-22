/**
 * @file base_log.h
 * @author LiYang
 * @brief common 模块日志打印，不依赖Logger。
 * @version 0.1
 * @date 2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef BASE_LOG_H
#define BASE_LOG_H

#ifdef NDEBUG

#ifndef BASE_LOG_INFO
#define BASE_LOG_INFO(fmt, ...) \
    do {                        \
    } while (0)
#endif  // BASE_LOG_INFO

#ifndef BASE_LOG_ERR
#define BASE_LOG_ERR(fmt, ...) \
    do {                       \
    } while (0)
#endif  // BASE_LOG_ERR

#else

#include <cstdio>
#include <cstring>

#ifndef BASE_LOG_INFO
#define BASE_LOG_INFO(fmt, ...)                                                                                       \
    do {                                                                                                              \
        fprintf(stdout, "[%s:%d] [%s] " fmt "\n", basename(__FILE__), __LINE__, __func__ __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)
#endif  // BASE_LOG_INFO

#ifndef BASE_LOG_ERR
#define BASE_LOG_ERR(fmt, ...)                                                                                        \
    do {                                                                                                              \
        fprintf(stderr, "[%s:%d] [%s] " fmt "\n", basename(__FILE__), __LINE__, __func__ __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)
#endif  // #define BASE_LOG_ERR
#endif

#endif  // BASE_LOG_H
