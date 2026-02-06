#ifndef LOGGINGL_LOG_SINK_H
#define LOGGINGL_LOG_SINK_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string_view>

#include "logging/details/common.h"
#include "logging/details/log_msg.h"
#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"

namespace logging {
class Sink {
public:
    Sink() = default;
    virtual ~Sink() = default;

    void log(const details::LogMsg& logMsg);

    void flush();

    [[nodiscard]] bool should_log(LogLevel level) const;
    void set_level(LogLevel level);

    void set_pattern(const std::string& pattern = DEFAULT_PATTERN,
                     const std::string& timePattern = DEFAULT_TIME_PATTERN);

    void set_formatter(std::unique_ptr<Formatter> formatter);

protected:
    virtual void sink_it(std::string_view message) = 0;
    virtual void flush_it() = 0;

private:
    std::atomic<LogLevel> _level = LogLevel::INFO;
    std::unique_ptr<Formatter> _formatter = std::make_unique<PatternFormatter>();
    std::mutex _mtx;
};
}  // namespace logging

struct sink_st {
    std::shared_ptr<logging::Sink> sinkPtr;
    explicit sink_st(std::shared_ptr<logging::Sink> sink) : sinkPtr(std::move(sink)) {}
};

#endif  // LOGGINGL_LOG_SINK_H
