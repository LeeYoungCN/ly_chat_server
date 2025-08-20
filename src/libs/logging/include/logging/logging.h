/**
 * @file logging.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#include <format>

#include "common/types/logging_types.h"
#include "logging/details/log_record.h"
#include "logging/logger.h"

namespace logging {

logging::Logger GetLogger(const std::string &name);
logging::Logger GetRootLogger();
void SetLogLevel(common::types::logging::LogLevel level);

void log(logging::details::LogSource resource, common::types::logging::LogLevel level, const char *fmt, ...);

template <typename... Args>
void log(logging::details::LogSource resource, common::types::logging::LogLevel level, std::format_string<Args...> fmt,
         Args &&...args)
{
    GetRootLogger().log();
}

}  // namespace logging

#endif  // LOGGING_LOGGING_H
