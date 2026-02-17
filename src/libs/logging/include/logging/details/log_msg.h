#pragma once
#ifndef LOGGINGL_DETAILS_LOG_MSG_H
#define LOGGINGL_DETAILS_LOG_MSG_H

#include <string>
#include <string_view>

#include "common/types/date_time_types.h"
#include "common/types/process_types.h"
#include "common/types/thread_types.h"
#include "logging/details/log_source.h"
#include "logging/log_level.h"

namespace logging::details {
struct LogMsg {
    std::string loggerName;
    LogLevel level{LogLevel::DEBUG};
    std::string data;
    LogSource source;
    common::date_time::TimestampMs timeStamp = 0;
    ThreadId threadId = 0;
    ProcessId procId = 0;

    LogMsg() = default;
    LogMsg(LogSource source, std::string_view loggerName, LogLevel level, std::string_view message);
};

}  // namespace logging::details
#endif  // LOGGINGL_DETAILS_LOG_MSG_H
