#include "logging/details/log_record.h"

#include <string_view>

#include "common/types/logging_types.h"
#include "common/utils/date_time_utils.h"
#include "common/utils/thread_utils.h"

namespace logging::details {

using namespace common::types::date_time;
using namespace common::types::thread;
using namespace common::utils::date_time;
using namespace common::utils::thread;
using namespace common::types::logging;

LogSource::LogSource(const char *file, int32_t line, const char *func) : file(file), line(line), func(func) {}

bool LogSource::isEmpty() const
{
    return file == nullptr || line < 0;
}

LogRecord::LogRecord(LogSource source, LogLevel level, std::string_view loggerName, std::string_view message)
    : source(source),
      loggerName(loggerName),
      level(level),
      message(message),
      time(GetCurrentTimestampMs()),
      tid(GetCurrentThreadId())
{
}

}  // namespace logging::details
