#ifndef LOGGINGL_LOG_SINK_H
#define LOGGINGL_LOG_SINK_H

#include <atomic>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "common/types/logging_types.h"
#include "logging/log_record.h"

namespace logging {
class Sink {
public:
    Sink() = default;
    virtual ~Sink() = default;

    void log(const LogRecord &logRecord)
    {
        constexpr uint32_t LOG_CONTENT_DEFAULT_LEN = 256;
        std::string content;
        content.reserve(LOG_CONTENT_DEFAULT_LEN);
        format(logRecord, content);
        write(content);
    }

    virtual void flush() = 0;

    void format(const LogRecord &logRecord, std::string& content);
    bool should_log(LogLevel level) { return level >= m_level; }
    void set_level(LogLevel level) { m_level = level; };

protected:
    virtual void write(std::string_view message) = 0;
    LogLevel level = LogLevel::LOG_LVL_INFO;

private:
    std::atomic<LogLevel> m_level = LogLevel::LOG_LVL_INFO;
};
}  // namespace logging

struct sink_st {
    std::shared_ptr<logging::Sink> sinkPtr;
    explicit sink_st(std::shared_ptr<logging::Sink> sink) : sinkPtr(std::move(sink)) {}
};

#endif  // LOGGINGL_LOG_SINK_H
