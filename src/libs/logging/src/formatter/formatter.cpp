#include "logging/formatter.h"

#include <format>
#include <string>

#include "common/utils/date_time_utils.h"
#include "common/utils/filesystem_utils.h"
#include "logging/details/log_record.h"

namespace logging {
using namespace common::utils::date_time;
using namespace common::types::logging;
using namespace common::utils::filesystem;
using namespace logging::details;

std::string Formatter::format(const details::LogRecord& record)
{
    return std::format("[{}] [{}] [Tid: {:#x}] [{}:{}] [{}] [{}] {}",
                       FormatTimeString(record.time, "%Y-%m-%d %H:%M:%S.%3f"),
                       logLevelToStr(record.level),
                       record.tid,
                       GetBaseName(record.source.file),
                       record.source.line,
                       record.source.func,
                       record.loggerName,
                       record.message);
}
}  // namespace logging
