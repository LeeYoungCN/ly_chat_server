#ifndef LOGGINGL_DETAILS_LOG_RECORD_H
#define LOGGINGL_DETAILS_LOG_RECORD_H
#include <cstdarg>
#include <string>
#include <string_view>
#include <utility>

#include "common/types/date_time_types.h"
#include "common/types/thread_types.h"
#include "common/utils/date_time_utils.h"
#include "common/utils/thread_utils.h"
#include "logging/details/log_level.h"
#include "logging/details/log_source.h"

namespace logging::details {
struct LogMsg {
    std::string_view loggerName;
    LogLevel level{LOG_LVL_DEBUG};
    std::string data;
    LogSource source;
    common::date_time::TimestampMs timeStamp = 0;
    ThreadId threadId = 0;

    LogMsg() = default;
    LogMsg(std::string_view loggerName, LogLevel level, std::string message, LogSource source)
        : loggerName(loggerName),
          level(level),
          data(std::move(message)),
          source(source),
          timeStamp(common::date_time::GetCurrentTimestampMs()),
          threadId(get_curr_thread_id())
    {
    }

    LogMsg(std::string_view loggerName, LogLevel level, std::string message)
        : LogMsg(loggerName, level, std::move(message), LogSource())
    {
    }
};

}  // namespace logging::details
#endif  // LOGGINGL_DETAILS_LOG_RECORD_H
