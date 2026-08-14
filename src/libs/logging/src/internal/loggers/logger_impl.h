#ifndef LOGGING_INTERNAL_LOGGERS_LOGGER_IMPL_H
#define LOGGING_INTERNAL_LOGGERS_LOGGER_IMPL_H

#include <atomic>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <vector>

#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/log_msg.h"
#include "logging/log_source.h"
#include "logging/sinks/sink.h"

namespace logging {
class LoggerImpl {
public:
    LoggerImpl() = delete;
    virtual ~LoggerImpl() = default;

    explicit LoggerImpl(std::string_view name);

    LoggerImpl(std::string_view name, const std::shared_ptr<logging::Sink>& sink);

    LoggerImpl(std::string_view name, const std::vector<std::shared_ptr<logging::Sink>>& sinks);

    LoggerImpl(std::string_view name,
               const std::initializer_list<std::shared_ptr<logging::Sink>>& sinks);

    [[nodiscard]] std::string_view name() const;
    [[nodiscard]] const std::vector<std::shared_ptr<logging::Sink>>& sinks() const;

    void set_level(LogLevel level);
    [[nodiscard]] LogLevel level() const;
    [[nodiscard]] bool should_log(LogLevel level) const;

    void flush_on(LogLevel level);
    [[nodiscard]] LogLevel flush_level() const;
    [[nodiscard]] bool should_flush(LogLevel level) const;

    void set_pattern(std::string_view pattern) const;
    void set_formatter(const std::unique_ptr<logging::Formatter>& formatter) const;

    void flush();
    void log(const LogSource& source, LogLevel level, std::string_view message);

    void backend_log(const LogMsg& logMsg);
    void backend_flush();

protected:
    virtual void log_it(const LogMsg& logMsg) = 0;
    virtual void flush_it() = 0;

protected:
    std::string _name;
    std::vector<std::shared_ptr<Sink>> _sinks;
    std::atomic<LogLevel> _level{LogLevel::INFO};
    std::atomic<LogLevel> _flushLevel{LogLevel::OFF};
};
}  // namespace logging
#endif  // LOGGING_INTERNAL_LOGGERS_LOGGER_IMPL_H
