/**
 * @file log_macros.h
 * @author your name (you@domain.com)
 * @brief 日志宏
 * @version 0.1
 * @date 2025-07-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef LOGGING_LOG_MACROS_H
#define LOGGING_LOG_MACROS_H
#include "common/common_macros.h"

#ifndef LOGGER
#define LOGGER(name) INST(logging::LoggerFactory).getLogger(name)
#endif  // LOGGER


#define LOG(name, level, fmt, ...) LOGGER(name)->log(LOG_SOURCE, level, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_DEBUG(name, fmt, ...) LOG(common::types::logging::LogLevel::DEBUG, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_INFO(name, fmt, ...) LOG(common::types::logging::LogLevel::INFO, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_WARN(name, fmt, ...) LOG(common::types::logging::LogLevel::WARNING, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_ERR(name, fmt, ...) LOG(common::types::logging::LogLevel::ERROR, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_FATAL(name, fmt, ...) LOG(common::types::logging::LogLevel::FATAL, fmt __VA_OPT__(, ) __VA_ARGS__)

#endif  // LOGGING_LOG_MACROS_H
