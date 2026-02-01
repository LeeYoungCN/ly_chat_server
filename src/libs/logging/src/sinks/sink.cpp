#include "logging/sinks/sink.h"

#include <format>

#include "common/utils/date_time_utils.h"
#include "common/utils/filesystem_utils.h"

namespace logging {

void Sink::format(const details::LogMsg &logRecord, std::string &content)
{
    auto it = std::back_inserter(content);

    it = std::format_to(it,
                        "[{}][{}]",
                        common::date_time::FormatTimeString(logRecord.timeStamp, "%Y-%m-%d %H:%M:%S.%3f"),
                        GetLogLvlStr(logRecord.level));

    if (logRecord.source.IsValid()) {
        it = std::format_to(
            it, "[{}:{}]", common::filesystem::GetFileName(logRecord.source.file), logRecord.source.line);
    }

    it = std::format_to(it, ": {}", logRecord.data);
}
}  // namespace logging
