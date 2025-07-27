#ifndef LOGGING_LOGGING_TYPES_H
#define LOGGING_LOGGING_TYPES_H

#include <string>
#include <thread>

#include "common/types/date_time_types.h"
#include "common/types/log_level.h"

namespace logging {
struct LogRecord {
    common::types::LogLevel level;
    const common::types::date_time::TimeStamp time;
    const char* file;
    int line;
    const char* func;
    std::thread::id threadId;
    std::string loggerName;
    std::string message;
};
}  // namespace logging

#endif  // LOGGING_LOGGING_TYPES_H
