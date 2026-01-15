#ifndef LOGGINGL_LOG_SINK_H
#define LOGGINGL_LOG_SINK_H

#include <atomic>
#include <string>

#include "common/types/logging_types.h"
#include "logging/log_record.h"

namespace logging {
class LogSink {
public:
    LogSink() = default;
    virtual ~LogSink() = default;

    virtual void log(const LogRecord &logRecord);
    virtual void flush() = 0;

    std::string format(const LogRecord &logRecord);
    bool should_log(LogLevel level) { return level >= m_level; }
    void set_level(LogLevel level) { m_level = level; };

protected:
    virtual void write(std::string message) = 0;

private:
    std::atomic<LogLevel> m_level = LogLevel::LOG_LVL_INFO;
};
}  // namespace logging

struct LogSinkSt {
    logging::LogSink *sinkPtr;
    explicit LogSinkSt(logging::LogSink *sink) : sinkPtr(sink) {}
};

#endif  // LOGGINGL_LOG_SINK_H
