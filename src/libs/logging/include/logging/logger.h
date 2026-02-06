#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <atomic>
#include <cstdarg>
#include <format>
#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "common/types/type_traits.h"
#include "common/utils/string_utils.h"
#include "logging/details/common.h"
#include "logging/details/log_msg.h"
#include "logging/details/log_source.h"
#include "logging/log_level.h"
#include "logging/sinks/sink.h"

namespace logging {
class Logger {
public:
    Logger() = default;
    virtual ~Logger() = default;

    explicit Logger(std::string name);

    Logger(std::string name, std::shared_ptr<Sink> sink);

    Logger(std::string name, std::vector<std::shared_ptr<Sink>> sinks);

    Logger(std::string name, std::initializer_list<std::shared_ptr<Sink>> sinks);

    template <typename It>
    Logger(std::string name, It begin, It end) : _name(std::move(name)), _sinks(begin, end)
    {
    }

    void set_level(LogLevel level);
    LogLevel level() const;
    bool should_log(LogLevel level) const;

    void flush_on(LogLevel level);
    LogLevel flush_level() const;
    bool should_flush(LogLevel level) const;

    const std::string& name() const;

    void set_pattern(const std::string& pattern = DEFAULT_PATTERN,
                     const std::string& timePattern = DEFAULT_TIME_PATTERN);
    void sef_formatter(std::unique_ptr<Formatter> formatter);

    void flush();

    // log function
    void log(details::LogSource source, LogLevel level, const char* format, va_list args);

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void log(details::LogSource source, LogLevel level, const T& message)
    {
        if (!should_log(level)) {
            return;
        }
        details::LogMsg logMsg(source, _name, level, common::string::type_to_string(message));
        sink_it(logMsg);
    }

    template <typename... Args>
    void log(details::LogSource source, LogLevel level, std::format_string<Args...> format,
             Args&&... args)
    {
        if (!should_log(level)) {
            return;
        }
        details::LogMsg logMsg(
            source, _name, level, std::format(format, std::forward<Args>(args)...));
        sink_it(logMsg);
    }

    DEFINE_LOGGER_LOG_FUNCTION(debug, logging::LogLevel::DEBUG)

    DEFINE_LOGGER_LOG_FUNCTION(info, logging::LogLevel::INFO)

    DEFINE_LOGGER_LOG_FUNCTION(warn, logging::LogLevel::WARN)

    DEFINE_LOGGER_LOG_FUNCTION(error, logging::LogLevel::ERROR)

    DEFINE_LOGGER_LOG_FUNCTION(fatal, logging::LogLevel::FATAL)

protected:
    virtual void sink_it(const details::LogMsg& logMsg);
    virtual void flush_it();

    void sinks_log_it(const details::LogMsg& logMsg);
    void sinks_flush_it();

private:
    std::string _name;
    std::vector<std::shared_ptr<Sink>> _sinks;
    std::atomic<LogLevel> _logLevel{LogLevel::INFO};
    std::atomic<LogLevel> _flushLevel{LogLevel::OFF};
};
}  // namespace logging

#endif  // LOGGING_LOGGER_H
