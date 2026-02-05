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

#ifndef LOGGING_DETAILS_LOG_LEVEL_H
#define LOGGING_DETAILS_LOG_LEVEL_H

#include <string_view>
namespace logging {
/**
 * @enum LogLevel
 * @brief 日志级别枚举
 */
enum LogLevel {
    DEBUG = 0,  ///< 调试信息（仅开发/测试环境使用，包含详细调试数据）
    INFO,       ///< 一般信息（正常运行时的流程性信息，如服务启动成功）
    WARN,       ///< 警告信息（不影响主流程但需关注的异常，如配置项缺失使用默认值）
    ERROR,      ///< 错误信息（功能异常但不导致程序退出，如单次接口调用失败）
    FATAL,      ///< 致命错误（导致程序无法继续运行的严重异常，如核心资源初始化失败）
    OFF         ///< 关闭日志功能
};

std::string_view log_level_to_string(LogLevel level, bool upper = true);

}  // namespace logging

#endif  // LOGGING_DETAILS_LOG_LEVEL_H
