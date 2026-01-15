#ifndef LOGGINGL_LOG_RECORD_H
#define LOGGINGL_LOG_RECORD_H
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <string_view>

#include "common/debug/debug_log.h"
#include "common/types/date_time_types.h"
#include "common/types/logging_types.h"
#include "common/types/thread_types.h"
#include "common/utils/date_time_utils.h"
#include "common/utils/thread_utils.h"
#include "logging/log_source.h"

namespace logging {
constexpr uint32_t LOG_RECORD_BUFFER_LEN = 128;

struct LogRecord {
    LogSrc source;
    std::string_view loggerName;
    LogLevel level{LOG_LVL_DEBUG};
    common::date_time::TimestampMs timeStamp{0};
    ThreadId threadId{0};
    char buffer[LOG_RECORD_BUFFER_LEN]{'\0'};

    LogRecord(std::string_view loggerName, LogLevel level, const char *fmt, va_list ap, LogSrc &&source)
        : source(source), loggerName(loggerName), level(level)
    {
        timeStamp = common::date_time::GetCurrentTimestampMs();
        threadId = get_curr_thread_id();
        auto len = vsnprintf(buffer, LOG_RECORD_BUFFER_LEN, fmt, ap);
        if (static_cast<uint32_t>(len) >= LOG_RECORD_BUFFER_LEN) {
            DEBUG_LOG_ERR("Log message overflow.");
        }
        buffer[LOG_RECORD_BUFFER_LEN - 1] = '\0';
    }

    LogRecord(std::string_view loggerName, LogLevel level, const char *fmt, va_list ap, const char *file = nullptr,
              int line = INVALID_LINE_NUM, const char *func = nullptr)
        : LogRecord(loggerName, level, fmt, ap, LogSrc(file, line, func))
    {
    }
};
}  // namespace logging
#endif  // LOGGINGL_LOG_RECORD_H
