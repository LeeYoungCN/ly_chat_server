/**
 * @file logging.h
 * @author your name (you@domain.com)
 * @brief 日志等级, 日志等级转字符串
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

namespace common::types {
/**
 * @enum LogLevel
 * @brief 日志级别枚举
 */
enum class LogLevel {
    DEBUG,     //  调试信息
    INFO,      // 一般信息
    WARNNING,  // 警告信息
    ERROR,     //  错误信息
    FATAL,     //  致命错误
};

inline const char* logLevelToStr(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNNING:
            return "WARNNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

}  // namespace common::types

#endif  // LOG_LEVEL_H
