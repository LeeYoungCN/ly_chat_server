#include "logging/c/logging_c.h"

#include <cstdarg>
#include <memory>

#include "c/common_c.h"
#include "logging/details/common.h"
#include "logging/details/registry.h"
#include "logging/log_level.h"
#include "utils/string_utils.h"

using namespace logging;
using namespace logging::details;
using namespace logging::c;

#define ROOT_LOGGER (INST(Registry).root_logger())

namespace {
void logging_log_it(const std::shared_ptr<Logger> &logger, const char *file, int line,
                    const char *func, LogLevel level, const char *format, va_list args)
{
    if (logger->should_log(level)) {
        logger->log(
            LogSource(file, line, func), level, utils::string::va_list_to_string(format, args));
    }
}
}  // namespace

extern "C" {
void logging_set_level(LoggingLevel level)
{
    ROOT_LOGGER->set_level(logging_to_log_level(level));
}

LoggingLevel logging_level()
{
    return log_to_logging_level(ROOT_LOGGER->level());
}

void logging_set_pattern(const char *pattern, const char *timePattern)
{
    ROOT_LOGGER->set_pattern(
        (pattern == nullptr ? FORMATTER_DEFAULT_PATTERN : pattern),
        (timePattern == nullptr ? FORMATTER_DEFAULT_TIME_PATTERN : timePattern));
}

void logging_flush()
{
    ROOT_LOGGER->flush();
}

void logging_log(const char *file, int line, const char *func, LoggingLevel level,
                 const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log_it(ROOT_LOGGER, file, line, func, logging_to_log_level(level), format, args);
    va_end(args);
}

void logging_debug(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log_it(ROOT_LOGGER, file, line, func, LogLevel::DEBUG, format, args);
    va_end(args);
}

void logging_info(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log_it(ROOT_LOGGER, file, line, func, LogLevel::INFO, format, args);
    va_end(args);
}

void logging_warn(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log_it(ROOT_LOGGER, file, line, func, LogLevel::WARN, format, args);
    va_end(args);
}

void logging_error(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log_it(ROOT_LOGGER, file, line, func, LogLevel::ERROR, format, args);
    va_end(args);
}

void logging_fatal(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log_it(ROOT_LOGGER, file, line, func, LogLevel::FATAL, format, args);
    va_end(args);
}
}
