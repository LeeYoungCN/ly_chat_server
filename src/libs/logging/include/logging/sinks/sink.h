#pragma once
#ifndef LOGGINGL_LOG_SINK_H
#define LOGGINGL_LOG_SINK_H

#include <atomic>
#include <memory>
#include <mutex>
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

    virtual void log(const details::LogMsg& logMsg) = 0;
    virtual void flush() = 0;

    void set_pattern(std::string_view pattern, std::string_view timePattern);
    void set_formatter(std::unique_ptr<Formatter> formatter);

    [[nodiscard]] bool should_log(LogLevel level) const;
    void set_level(LogLevel level);
    [[nodiscard]] LogLevel level() const;

protected:
    std::atomic<LogLevel> _level = LogLevel::INFO;
    std::unique_ptr<Formatter> _formatter{std::make_unique<PatternFormatter>()};
    std::mutex _sinkMtx;
};
}  // namespace logging

#endif  // LOGGINGL_LOG_SINK_H
