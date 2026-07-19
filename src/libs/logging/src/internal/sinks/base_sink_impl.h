#pragma once
#ifndef LOGGING_INTERNAL_SINKS_BASE_SINK_IMPL_H
#define LOGGING_INTERNAL_SINKS_BASE_SINK_IMPL_H
#include <atomic>
#include <memory>
#include <mutex>
#include <string_view>

#include "logging/details/log_msg.h"
#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"
#include "logging/sinks/sink.h"

namespace logging {
class BaseSinkImpl : public Sink {
public:
    BaseSinkImpl() = default;
    ~BaseSinkImpl() override;

    void log(const details::LogMsg& logMsg) override;
    void flush() override;

    void set_pattern(std::string_view pattern) override;
    void set_formatter(std::unique_ptr<Formatter> formatter) override;

    [[nodiscard]] bool should_log(LogLevel level) const override;
    void set_level(LogLevel level) override;
    [[nodiscard]] LogLevel level() const override;

protected:
    explicit BaseSinkImpl(std::string_view parameter);
    virtual void log_it(const details::LogMsg& logMsg) = 0;
    virtual void flush_it() = 0;

protected:
    std::atomic<LogLevel> _level{LogLevel::INFO};
    std::unique_ptr<Formatter> _formatter{std::make_unique<PatternFormatter>()};
    std::mutex _sinkMtx;
    std::string _paramStr{"unknown"};
};
}  // namespace logging

#endif  // LOGGING_INTERNAL_SINKS_BASE_SINK_IMPL_H
