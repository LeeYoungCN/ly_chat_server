#include "internal/log_console_sink.h"
#include <cstdio>

#include "internal/log_record.h"

namespace logging {
LogConsoleSink::LogConsoleSink(ConsoleType type)
{
    if (type == ConsoleType::CONSOLE_STDERR) {
        m_stream = stderr;
    } else {
        m_stream = stdout;
    }
}

void LogConsoleSink::Init() {}
void LogConsoleSink::Close() {}

void LogConsoleSink::Log(const LogRecord *logRecord)
{
    fprintf(m_stream, "%s", logRecord->buffer);
}
}  // namespace logging
