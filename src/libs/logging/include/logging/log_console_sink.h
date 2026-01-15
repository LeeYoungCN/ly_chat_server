#ifndef LOGGINGL_LOG_CONSOLE_SINK_H
#define LOGGINGL_LOG_CONSOLE_SINK_H

#include <cstdio>

#include "common/types/error_code_types.h"
#include "logging/log_record.h"
#include "logging/log_sink.h"
#include "logging/logging.h"

namespace logging {
class LogConsoleSink : public LogSink {
public:
    LogConsoleSink() = default;
    ~LogConsoleSink() override = default;
    explicit LogConsoleSink(ConsoleType type);
    ErrorCode init() override;
    void close() override;
    void log(const LogRecord &logRecord) override;

private:
    ConsoleType m_type{ConsoleType::CONSOLE_STDOUT};
    FILE *m_stream = stdout;
};
}  // namespace logging
#endif  // LOGGINGL_LOG_CONSOLE_SINK_H
