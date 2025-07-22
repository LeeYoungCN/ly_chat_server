#ifndef LOGGING_TYPES_H
#define LOGGING_TYPES_H

#include <string>
#include <thread>

#include "common/types/log_level.h"
#include "common/types/time_common.h"


namespace logging {
struct LogRecord {
    common::types::LogLevel level;
    const common::types::TimeStamp time;
    const char* file;
    int line;
    const char* func;
    std::thread::id threadId;
    std::string loggerName;
    std::string message;
};
}  // namespace logging

#endif  // LOGGING_TYPES_H
