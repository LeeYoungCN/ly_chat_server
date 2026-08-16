#ifndef ORIGIN_LOGGING_LOGGERS_LOGGER_BASE_H
#define ORIGIN_LOGGING_LOGGERS_LOGGER_BASE_H
#include <memory>
#include <string_view>
#include <vector>

#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/log_source.h"
#include "logging/loggers/logger.h"
#include "logging/sinks/sink.h"

namespace origin::logging {
class LoggerImpl;
class LoggerBase : public Logger {
public:
    LoggerBase() = delete;
    explicit LoggerBase(std::shared_ptr<LoggerImpl> pImpl);
    ~LoggerBase() override;

    [[nodiscard]] std::string_view name() const override;
    [[nodiscard]] std::vector<std::shared_ptr<Sink>> sinks() const override;

    void set_level(LogLevel level) const override;
    [[nodiscard]] LogLevel level() const override;
    [[nodiscard]] bool should_log(LogLevel level) const override;

    void flush_on(LogLevel level) const override;
    [[nodiscard]] LogLevel flush_level() const override;
    [[nodiscard]] bool should_flush(LogLevel level) const override;

    void set_pattern(std::string_view pattern) const override;
    void set_formatter(const std::unique_ptr<Formatter>& formatter) const override;

    void flush() override;

protected:
    void log_it(const LogSource& source, LogLevel level, std::string_view message) override;

protected:
    void throw_if_pimpl_null() const;
    std::shared_ptr<LoggerImpl> _pImpl;
};
}  // namespace origin::logging
#endif  // ORIGIN_LOGGING_LOGGERS_LOGGER_BASE_H
