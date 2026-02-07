#include "c/logging_level_c.h"

#include "logging/log_level.h"
#include "logging/logging_c.h"

namespace logging::c {
LogLevel logging_to_log_level(LoggingLevel level)
{
    switch (level) {
        case LOG_LEVEL_DEBUG:
            return LogLevel::DEBUG;
        case LOG_LEVEL_INFO:
            return LogLevel::INFO;
        case LOG_LEVEL_WARN:
            return LogLevel::WARN;
        case LOG_LEVEL_ERROR:
            return LogLevel::ERROR;
        case LOG_LEVEL_FATAL:
            return LogLevel::FATAL;
        case LOG_LEVEL_OFF:
        default:
            return LogLevel::OFF;
    }
}

LoggingLevel log_to_logging_level(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return LOG_LEVEL_DEBUG;
        case LogLevel::INFO:
            return LOG_LEVEL_INFO;
        case LogLevel::WARN:
            return LOG_LEVEL_WARN;
        case LogLevel::ERROR:
            return LOG_LEVEL_ERROR;
        case LogLevel::FATAL:
            return LOG_LEVEL_FATAL;
        case LogLevel::OFF:
        default:
            return LOG_LEVEL_OFF;
    }
}
}  // namespace logging::c
