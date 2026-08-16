#include <cstdarg>
#include <memory>

#include "c/common_c.h"
#include "internal/registry.h"
#include "logging/c/logging_c.h"
#include "logging/log_level.h"
#include "utils/string_utils.h"

using namespace origin::logging;
using namespace origin::logging::c;

#define ROOT_LOGGER (INST(Registry).root_logger())

namespace {
void origin_log_it(const std::shared_ptr<Logger> &logger, const char *file, int line,
                   const char *func, LogLevel level, const char *format, va_list args)
{
    if (logger->should_log(level)) {
        logger->log(LogSource(file, line, func),
                    level,
                    origin::utils::string::va_list_to_string(format, args));
    }
}
}  // namespace

extern "C" {
void origin_set_level(OriginLogLevel level)
{
    ROOT_LOGGER->set_level(c_to_cpp_log_level(level));
}

OriginLogLevel origin_level()
{
    return cpp_to_c_log_level(ROOT_LOGGER->level());
}

void origin_set_pattern(const char *pattern)
{
    if (pattern == nullptr) {
        return;
    }
    ROOT_LOGGER->set_pattern(pattern);
}

void origin_flush()
{
    ROOT_LOGGER->flush();
}

void origin_log(const char *file, int line, const char *func, OriginLogLevel level,
                const char *format, ...)
{
    va_list args;
    va_start(args, format);
    origin_log_it(ROOT_LOGGER, file, line, func, c_to_cpp_log_level(level), format, args);
    va_end(args);
}

void origin_debug(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    origin_log_it(ROOT_LOGGER, file, line, func, LogLevel::DEBUG, format, args);
    va_end(args);
}

void origin_info(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    origin_log_it(ROOT_LOGGER, file, line, func, LogLevel::INFO, format, args);
    va_end(args);
}

void origin_warn(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    origin_log_it(ROOT_LOGGER, file, line, func, LogLevel::WARN, format, args);
    va_end(args);
}

void origin_error(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    origin_log_it(ROOT_LOGGER, file, line, func, LogLevel::ERR, format, args);
    va_end(args);
}

void origin_fatal(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    origin_log_it(ROOT_LOGGER, file, line, func, LogLevel::FATAL, format, args);
    va_end(args);
}
}
