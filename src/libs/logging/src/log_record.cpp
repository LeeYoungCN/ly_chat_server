#include "logging/log_record.h"

// #include "common/types/date_time_types.h"
#include "common/types/logging_types.h"
// #include "common/types/thread_types.h"
#include "common/utils/date_time_utils.h"
#include "common/utils/thread_utils.h"

namespace logging {

using namespace common::types::date_time;
using namespace common::types::thread;
using namespace common::utils::date_time;
using namespace common::utils::thread;

LogSource::LogSource(const char *file, int32_t line, const char *func) : file(file), line(line), func(func) {}

bool LogSource::isEmpty() const
{
    return file == nullptr;
}

LogRecord::LogRecord(LogSource source, common::types::logging::LogLevel level, std::string_view loggerName,
                     std::string_view message)
    : source(source),
      loggerName(loggerName),
      level(level),
      message(message),
      time(GetCurrentTimestampMs()),
      tid(GetCurrentThreadId())
{
}

}  // namespace logging
