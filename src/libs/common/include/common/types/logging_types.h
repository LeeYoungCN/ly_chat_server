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

namespace common::types::logging {
/**
 * @enum LogLevel
 * @brief 日志级别枚举
 */
enum class LogLevel {
    DEBUG,  ///< 调试信息（仅开发/测试环境使用，包含详细调试数据）
    INFO,   ///< 一般信息（正常运行时的流程性信息，如服务启动成功）
    WARN,   ///< 警告信息（不影响主流程但需关注的异常，如配置项缺失使用默认值）
    ERR,    ///< 错误信息（功能异常但不导致程序退出，如单次接口调用失败）
    FATAL   ///< 致命错误（导致程序无法继续运行的严重异常，如核心资源初始化失败）
};

inline const char* logLevelToStr(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

}  // namespace common::types::logging

#endif  // COMMON_TYPES_LOGGING_TYPES_H
