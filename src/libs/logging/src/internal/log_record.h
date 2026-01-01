#ifndef LOGGING_INTERNAL_LOG_RECORD_H
#define LOGGING_INTERNAL_LOG_RECORD_H
#include "common/types/date_time_types.h"
#include "common/types/logging_types.h"
#include "common/types/thread_types.h"

namespace logging {
#define LOG_RECORD_BUFFER_LEN (128)
struct LogRecord {
    const char *file{nullptr};
    int line{-1};
    const char *func{nullptr};
    common::date_time::TimestampMs timeStamp{0};
    LogLevel level{LOG_LVL_DEBUG};
    ThreadId threadId{0};
    char buffer[LOG_RECORD_BUFFER_LEN]{'\0'};
};
}  // namespace logging
#endif  // LOGGING_INTERNAL_LOG_RECORD_H
