#include "logging/logging.h"

#include <atomic>
#include <cstdarg>
#include <vector>

#include "common/common_error_code.h"
#include "common/debug/debug_log.h"
#include "common/types/error_code_types.h"
#include "common/types/logging_types.h"
#include "logging/log_basic_file_sink.h"
#include "logging/log_console_sink.h"
#include "logging/log_record.h"
#include "logging/log_sink.h"

namespace {
std::vector<logging::LogSink *> g_sinkList;
std::atomic<LogLevel> g_allowedLevel{LOG_LVL_INFO};
}  // namespace

extern "C" {
LogSinkSt *logging_get_console_sink(ConsoleType type)
{
    return new LogSinkSt(new logging::LogConsoleSink(type));
}

LogSinkSt *logging_get_file_sink(const char *file, bool overwrite)
{
    return new LogSinkSt(new logging::LogBasicFileSink(file, overwrite));
}

void logging_register_sink(LogSinkSt *sink)
{
    g_sinkList.push_back(sink->sinkPtr);
    delete sink;
}

void logging_set_level(LogLevel level)
{
    g_allowedLevel = level;
}

ErrorCode logging_init()
{
    ErrorCode errcode = ERR_COMM_SUCCESS;
    for (auto sink : g_sinkList) {
        errcode = sink->init();
    }
    return errcode;
}

void logging_close()
{
    for (auto sink : g_sinkList) {
        sink->close();
    }
    while (!g_sinkList.empty()) {
        auto sink = *(g_sinkList.end() - 1);
        delete sink;
        g_sinkList.pop_back();
    }
}

void logging_log(const char *file, int line, const char *func, LogLevel level, const char *format, ...)
{
    if (level < g_allowedLevel) {
        return;
    }
    va_list ap;
    va_start(ap, format);
    logging::LogRecord record("Default", level, format, ap, file, line, func);
    va_end(ap);

    for (auto sink : g_sinkList) {
        sink->log(record);
    }
}
}
