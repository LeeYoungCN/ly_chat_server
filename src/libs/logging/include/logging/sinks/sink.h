#pragma once
#ifndef LOGGING_SINKS_SINK_H
#define LOGGING_SINKS_SINK_H

#include <memory>
#include <string_view>

#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/log_msg.h"

namespace logging {
class Sink {
public:
    Sink() = default;
    virtual ~Sink() = default;

    virtual void log(const LogMsg& logMsg) = 0;
    virtual void flush() = 0;

    virtual void set_pattern(std::string_view pattern) = 0;
    virtual void set_formatter(std::unique_ptr<Formatter> formatter) = 0;

    [[nodiscard]] virtual bool should_log(LogLevel level) const = 0;
    virtual void set_level(LogLevel level) = 0;
    [[nodiscard]] virtual LogLevel level() const = 0;
};
}  // namespace logging

#endif  // LOGGING_SINKS_SINK_H
