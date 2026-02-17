#pragma once
#ifndef LOGGINGL_LOG_SINK_H
#define LOGGINGL_LOG_SINK_H

#include <memory>
#include <mutex>
#include <string_view>

#include "common/compiler/macros.h"
#include "logging/details/log_msg.h"
#include "logging/formatters/formatter.h"
#include "logging/log_level.h"

namespace logging {
class COMMON_API Sink {
public:
    Sink();
    virtual ~Sink();

    virtual void log(const details::LogMsg& logMsg) = 0;
    virtual void flush() = 0;

    void set_pattern(std::string_view pattern, std::string_view timePattern);
    void set_formatter(std::unique_ptr<Formatter> formatter);

    [[nodiscard]] bool should_log(LogLevel level) const;
    void set_level(LogLevel level);
    [[nodiscard]] LogLevel level() const;

protected:
    std::mutex& sink_mutex();
    std::unique_ptr<Formatter>& formatter();

private:
    struct Impl;
    std::unique_ptr<Impl> _pimpl;
};
}  // namespace logging

#endif  // LOGGINGL_LOG_SINK_H
