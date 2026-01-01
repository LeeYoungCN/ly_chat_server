/**
 * @file logging_types.h
 * @author LiYang
 * @brief 日志等级, 日志等级转字符串
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#ifndef COMMON_TYPES_LOGGING_TYPES_H
#define COMMON_TYPES_LOGGING_TYPES_H

/**
 * @enum LogLevel
 * @brief 日志级别枚举
 */
typedef enum {
    LOG_LVL_DEBUG = 1,  ///< 调试信息（仅开发/测试环境使用，包含详细调试数据）
    LOG_LVL_INFO,       ///< 一般信息（正常运行时的流程性信息，如服务启动成功）
    LOG_LVL_WARN,       ///< 警告信息（不影响主流程但需关注的异常，如配置项缺失使用默认值）
    LOG_LVL_ERR,        ///< 错误信息（功能异常但不导致程序退出，如单次接口调用失败）
    LOG_LVL_FATAL       ///< 致命错误（导致程序无法继续运行的严重异常，如核心资源初始化失败）
} LogLevel;

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
inline static const char* GetLogLvlStr(LogLevel level)
{
    switch (level) {
        case LOG_LVL_DEBUG:
            return "DEBUG";
        case LOG_LVL_INFO:
            return "INFO";
        case LOG_LVL_WARN:
            return "WARN";
        case LOG_LVL_ERR:
            return "ERROR";
        case LOG_LVL_FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // COMMON_TYPES_LOGGING_TYPES_H
