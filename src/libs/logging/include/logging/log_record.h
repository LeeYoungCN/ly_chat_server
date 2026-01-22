#ifndef LOGGINGL_LOG_RECORD_H
#define LOGGINGL_LOG_RECORD_H
#include <cstdarg>
#include <cstdint>
#include <string>
#include <string_view>

#include "common/types/date_time_types.h"
#include "common/types/logging_types.h"
#include "common/types/thread_types.h"
#include "common/utils/date_time_utils.h"
#include "common/utils/thread_utils.h"
#include "logging/log_source.h"

namespace logging {
constexpr uint32_t LOG_RECORD_BUFFER_LEN = 256;

struct LogRecord {
    std::string_view loggerName;
    LogLevel level{LOG_LVL_DEBUG};
    std::string message;
    LogSource source;
    common::date_time::TimestampMs timeStamp = common::date_time::GetCurrentTimestampMs();
    ThreadId threadId = get_curr_thread_id();

    LogRecord(std::string_view loggerName, LogLevel level, std::string message, LogSource source)
        : loggerName(loggerName), level(level), message(std::move(message)), source(source) { }

    LogRecord(std::string_view loggerName, LogLevel level, std::string message)
        : loggerName(loggerName), level(level), message(std::move(message)) { }
};
}  // namespace logging
#endif  // LOGGINGL_LOG_RECORD_H
