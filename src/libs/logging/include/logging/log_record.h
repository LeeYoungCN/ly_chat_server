#ifndef LOGGING_LOGGING_TYPES_H
#define LOGGING_LOGGING_TYPES_H

#include <string_view>

#include "common/types/date_time_types.h"
#include "common/types/logging_types.h"
#include "common/types/thread_types.h"

namespace logging {

struct LogSource {
    const char *file{nullptr};
    int32_t line{-1};
    const char *func{nullptr};

    [[nodiscard]] bool isEmpty() const;
    LogSource() = default;
    LogSource(const char *file, int32_t line, const char *func);
};

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
}  // namespace logging

#endif  // LOGGING_LOGGING_TYPES_H
