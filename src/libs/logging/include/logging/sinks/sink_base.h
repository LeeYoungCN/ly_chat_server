#pragma once
#ifndef LOGGING_SINKS_BASE_SINK_H
#define LOGGING_SINKS_BASE_SINK_H

#include <memory>
#include <string_view>

#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/log_msg.h"
#include "logging/sinks/sink.h"

namespace logging {
class SinkBase : public Sink {
public:
    SinkBase() = delete;
    ~SinkBase() override;
    explicit SinkBase(std::unique_ptr<Sink> pImpl);

    void log(const LogMsg& logMsg) override;
    void flush() override;

    void set_pattern(std::string_view pattern) override;
    void set_formatter(std::unique_ptr<Formatter> formatter) override;

    [[nodiscard]] bool should_log(LogLevel level) const override;
    void set_level(LogLevel level) override;
    [[nodiscard]] LogLevel level() const override;
    [[nodiscard]] std::string_view param_str() const override;

protected:
    void throw_if_pimpl_null() const;
    std::unique_ptr<Sink> _pImpl;
};
}  // namespace logging

#endif  // LOGGING_SINKS_BASE_SINK_H
