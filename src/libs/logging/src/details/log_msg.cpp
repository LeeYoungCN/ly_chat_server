#include "logging/details/log_msg.h"

#include <string>
#include <string_view>
#include <utility>

#include "common/utils/date_time_utils.h"
#include "common/utils/process_utils.h"
#include "common/utils/thread_utils.h"
#include "logging/details/log_source.h"
#include "logging/log_level.h"

namespace logging::details {
LogMsg::LogMsg(LogSource source, std::string_view loggerName, LogLevel level,
               std::string_view message)
    : loggerName(loggerName),
      level(level),
      data(message),
      source(std::move(source)),
      timeStamp(common::date_time::GetCurrentTimestampMs()),
      threadId(get_curr_thread_id()),
      procId(common::process::get_curr_proc_id())
{
}
}  // namespace logging::details
