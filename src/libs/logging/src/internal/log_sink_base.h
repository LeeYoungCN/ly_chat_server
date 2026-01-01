#ifndef LOGGING_INTERNAL_LOG_SINK_BASE_H
#define LOGGING_INTERNAL_LOG_SINK_BASE_H

#include <atomic>
#include <mutex>

#include "common/types/logging_types.h"
#include "internal/log_record.h"
#include "log_record.h"

namespace logging {
class LogSinkBase {
public:
    LogSinkBase() = default;
    virtual ~LogSinkBase() = default;
    virtual void Init() = 0;
    virtual void Close() = 0;
    virtual void Log(const LogRecord *logRecord) = 0;
    bool ShouldLog(LogLevel level) { return level >= m_logLevel; }
    void SetLevel(LogLevel level) { m_logLevel = level; };
    void FormatString();

protected:
    std::atomic<LogLevel> m_logLevel {LOG_LVL_INFO};
    std::mutex m_logMutex;
};
}  // namespace logging
#endif  // LOGGING_INTERNAL_LOG_SINK_BASE_H
