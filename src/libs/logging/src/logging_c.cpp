#include "logging/logging_c.h"

#include <cstdarg>
#include <cstdio>

#include "common/utils/string_utils.h"
#include "logging/details/registry.h"
#include "logging/log_level.h"
#include "logging/logging.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/stdout_sink.h"

namespace {

void logging_log(const char *file, int line, const char *func, logging::LogLevel level,
                 const char *format, va_list args)
{
    logging::log(logging::details::LogSource(file, line, func),
                 level,
                 common::string::va_list_to_string(format, args));
}
}  // namespace

using namespace logging;

extern "C" {
sink_st *logging_get_stdout_sink(FILE *file)
{
    return new sink_st(std::make_shared<StdoutSink>(file));
}

sink_st *logging_get_basic_file_sink(const char *file, bool overwrite)
{
    return new sink_st(std::make_shared<BasicFileSink>((file == nullptr ? "" : file), overwrite));
}

void logging_set_level(LoggingLogLevel level)
{
    INST(details::Registry).root_logger_raw()->set_level(static_cast<LogLevel>(level));
}

void logging_flush()
{
    INST(details::Registry).root_logger_raw()->flush();
}

void logging_debug(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::DEBUG, format, args);
    va_end(args);
}

void logging_info(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::INFO, format, args);
    va_end(args);
}

void logging_warn(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::WARN, format, args);
    va_end(args);
}

void logging_error(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::ERROR, format, args);
    va_end(args);
}

void logging_fatal(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::FATAL, format, args);
    va_end(args);
}
}
