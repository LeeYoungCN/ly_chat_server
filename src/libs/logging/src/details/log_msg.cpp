#include "logging/details/log_msg.h"

#include <string>
#include <string_view>
#include <utility>

#include "logging/details/log_source.h"
#include "logging/log_level.h"
#include "utils/date_time_utils.h"
#include "utils/process_utils.h"
#include "utils/thread_utils.h"

namespace logging::details {
LogMsg::LogMsg(LogSource source, std::string_view loggerName, LogLevel level,
               std::string_view message)
    : loggerName(loggerName),
      level(level),
      data(message),
      source(std::move(source)),
      timeStamp(utils::date_time::GetCurrentTimestampMs()),
      threadId(get_curr_thread_id()),
      procId(utils::process::get_curr_proc_id())
{
}
}  // namespace logging::details
