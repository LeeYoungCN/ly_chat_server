#ifndef ORIGIN_LOGGING_LOGGERS_INTERNAL_LOGGER_IMPL_H
#define ORIGIN_LOGGING_LOGGERS_INTERNAL_LOGGER_IMPL_H

#include <atomic>
#include <initializer_list>
#include <memory>
#include <string_view>
#include <vector>

#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/log_msg.h"
#include "logging/log_source.h"
#include "logging/loggers/logger.h"
#include "logging/sinks/sink.h"

namespace origin::logging {
class LoggerImpl : public Logger {
public:
    LoggerImpl() = delete;
    virtual ~LoggerImpl() = default;

    explicit LoggerImpl(std::string_view name);

    LoggerImpl(std::string_view name, const std::shared_ptr<Sink>& sink);

    LoggerImpl(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks);

    LoggerImpl(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks);

    [[nodiscard]] std::string_view name() const override;
    [[nodiscard]] const std::vector<std::shared_ptr<Sink>>& sinks() const override;

    void set_level(LogLevel level) override;
    [[nodiscard]] LogLevel level() const override;
    [[nodiscard]] bool should_log(LogLevel level) const override;

    void flush_on(LogLevel level) override;
    [[nodiscard]] LogLevel flush_level() const override;
    [[nodiscard]] bool should_flush(LogLevel level) const override;

    void set_pattern(std::string_view pattern) const override;
    void set_formatter(const std::unique_ptr<Formatter>& formatter) const override;

    void flush() override;
    void force_log(const LogSource& source, LogLevel level, std::string_view message) override;

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
}  // namespace origin::logging
#endif  // ORIGIN_LOGGING_LOGGERS_INTERNAL_LOGGER_IMPL_H
