#include "logging/log_sink.h"

#include <cstdarg>
#include <cstdio>
#include <format>

#include "common/utils/date_time_utils.h"
#include "common/utils/filesystem_utils.h"

namespace logging {
void LogSink::log(const LogRecord &logRecord)
{
    write(format(logRecord));
}

std::string LogSink::format(const LogRecord &logRecord)
{
    return std::format("[{}] [{}] [Tid: {:#x}] [{}:{}] [{}] {}",
                       common::date_time::FormatTimeString(logRecord.timeStamp, "%Y-%m-%d %H:%M:%S.%3f"),
                       GetLogLvlStr(logRecord.level),
                       logRecord.threadId,
                       common::utils::filesystem::GetFileName(logRecord.source.file),
                       logRecord.source.line,
                       logRecord.source.func,
                       logRecord.buffer);
}
}  // namespace logging
