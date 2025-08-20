#ifndef LOGGING_DETAILS_LOG_RECORD_H
#define LOGGING_DETAILS_LOG_RECORD_H

#include <string_view>

#include "common/types/date_time_types.h"
#include "common/types/logging_types.h"
#include "common/types/thread_types.h"
#include "logging/details/log_source.h"

namespace logging::details {

struct LogRecord {
    LogSource source;
    std::string_view loggerName;
    common::types::logging::LogLevel level{common::types::logging::LogLevel::INFO};
    std::string_view message;
    common::types::date_time::TimestampMs time{0};
    common::types::thread::ThreadId tid{0};

    LogRecord() = default;

    LogRecord(LogSource source, common::types::logging::LogLevel level, std::string_view loggerName,
              std::string_view message);
};
}  // namespace logging::details

#endif  // LOGGING_DETAILS_LOG_RECORD_H
