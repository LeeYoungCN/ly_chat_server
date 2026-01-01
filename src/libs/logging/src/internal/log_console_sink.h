#ifndef LOGGING_INTERNAL_LOG_CONSOLE_SINK_H
#define LOGGING_INTERNAL_LOG_CONSOLE_SINK_H

#include <cstdio>

#include "internal/log_record.h"
#include "internal/log_sink_base.h"
#include "logging/logging.h"

namespace logging {
class LogConsoleSink : public LogSinkBase {
public:
    LogConsoleSink() = default;
    ~LogConsoleSink() override = default;
    explicit LogConsoleSink(ConsoleType type);
    void Init() override;
    void Close() override;
    void Log(const LogRecord *logRecord) override;

private:
    FILE *m_stream{stdout};
};
}  // namespace logging
#endif  // LOGGING_INTERNAL_LOG_CONSOLE_SINK_H
