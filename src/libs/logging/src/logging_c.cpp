#include "logging/logging_c.h"

#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <stdexcept>
#include <string>

#include "common/types/logging_types.h"
#include "logging/logger.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/sink.h"
#include "logging/sinks/stdout_sink.h"

namespace {
logging::Logger root;

void logging_log(const char *file, int line, const char *func, LogLevel level, const char *format, va_list args)
{
    if (!root.should_log(level)) {
        return;
    }

    va_list argsCopy;

    va_copy(argsCopy, args);
    int len = vsnprintf(nullptr, 0, format, argsCopy);
    va_end(argsCopy);

    if (len < 0) {
        throw std::runtime_error("vsnprintf failed.");
    }

    std::string message(static_cast<uint32_t>(len), '\0');
    vsnprintf(message.data(), static_cast<uint32_t>(len) + 1, format, args);
    va_end(args);

    root.log(logging::details::LogSource(file, line, func), level, message);
}
}  // namespace

extern "C" {
sink_st *logging_get_stdout_sink(FILE *file)
{
    return new sink_st(std::make_shared<logging::StdoutSink>(file));
}

sink_st *logging_get_basic_file_sink(const char *file, bool overwrite)
{
    return new sink_st(std::make_shared<logging::BasicFileSink>((file == nullptr ? "" : file), overwrite));
}

void logging_add_sink(sink_st *sink)
{
    root.add_sink(sink->sinkPtr);
    delete sink;
}

void logging_set_level(LogLevel level)
{
    root.set_log_level(level);
}

void logging_flush()
{
    root.flush();
}

void logging_debug(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::LOG_LVL_DEBUG, format, args);
    va_end(args);
}

void logging_info(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::LOG_LVL_INFO, format, args);
    va_end(args);
}

void logging_warn(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::LOG_LVL_WARN, format, args);
    va_end(args);
}

void logging_error(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::LOG_LVL_ERR, format, args);
    va_end(args);
}

void logging_fatal(const char *file, int line, const char *func, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    logging_log(file, line, func, LogLevel::LOG_LVL_FATAL, format, args);
    va_end(args);
}
}
