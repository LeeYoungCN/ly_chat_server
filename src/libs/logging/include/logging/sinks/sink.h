#ifndef LOGGINGL_LOG_SINK_H
#define LOGGINGL_LOG_SINK_H

#include <atomic>
#include <memory>
#include <string_view>

#include "logging/details/log_msg.h"
#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"

namespace logging {
class Sink {
public:
    Sink() = default;
    virtual ~Sink() = default;

    void log(const details::LogMsg &logMsg);

    virtual void flush() = 0;

    bool should_log(LogLevel level) const { return level >= _level; }
    void set_level(LogLevel level) { _level = level; };

protected:
    virtual void write(std::string_view message) = 0;

private:
    std::atomic<LogLevel> _level = LogLevel::INFO;
    std::unique_ptr<Formatter> _formatter = std::make_unique<PatternFormatter>();
};
}  // namespace logging

struct sink_st {
    std::shared_ptr<logging::Sink> sinkPtr;
    explicit sink_st(std::shared_ptr<logging::Sink> sink) : sinkPtr(std::move(sink)) {}
};

#endif  // LOGGINGL_LOG_SINK_H
