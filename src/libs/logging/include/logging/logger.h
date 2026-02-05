#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <atomic>
#include <cstdarg>
#include <format>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include "common/types/type_traits.h"
#include "common/utils/string_utils.h"
#include "logging/log_level.h"
#include "logging/details/log_msg.h"
#include "logging/details/log_source.h"
#include "logging/sinks/sink.h"

namespace logging {
class Logger {
public:
    Logger() = default;
    explicit Logger(std::string name);
    virtual ~Logger();

    void set_log_level(LogLevel level);
    void set_flush_level(LogLevel level);
    void set_name(std::string name);
    std::string_view get_name() const;

    bool should_log(LogLevel level) const;

    void add_sink(const std::shared_ptr<Sink>& sink);

    void flush();

    void log(details::LogSource source, LogLevel level, const char* format, va_list args);

    template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void log(details::LogSource source, LogLevel level, const T& message)
    {
        if (!should_log(level)) {
            return;
        }
        details::LogMsg logMsg(source, _name, level, common::string::type_to_string(message));
        log_it(std::move(logMsg));
    }

    template <typename... Args>
    void log(details::LogSource source, LogLevel level, std::format_string<Args...> format, Args&&... args)
    {
        if (!should_log(level)) {
            return;
        }
        details::LogMsg logMsg(source, _name, level, std::format(format, std::forward<Args>(args)...));
        log_it(std::move(logMsg));
    }

    template <typename... Args>
    void debug(std::format_string<Args...> format, Args&&... args)
    {
        debug(LOG_SRC_EMPTY, format, std::forward<Args>(args)...);
    }

    template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void debug(const T& message)
    {
        debug(LOG_SRC_EMPTY, message);
    }

    template <typename... Args>
    void debug(details::LogSource source, std::format_string<Args...> format, Args&&... args)
    {
        log(source, LogLevel::DEBUG, format, std::forward<Args>(args)...);
    }

    template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void debug(details::LogSource source, const T& message)
    {
        log(source, LogLevel::DEBUG, message);
    }

    // info
    template <typename... Args>
    void info(std::format_string<Args...> format, Args&&... args)
    {
        info(LOG_SRC_EMPTY, format, std::forward<Args>(args)...);
    }

    template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void info(const T& message)
    {
        info(LOG_SRC_EMPTY, message);
    }

    template <typename... Args>
    void info(details::LogSource source, std::format_string<Args...> format, Args&&... args)
    {
        log(source, LogLevel::INFO, format, std::forward<Args>(args)...);
    }

    template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void info(details::LogSource source, const T& message)
    {
        log(source, LogLevel::INFO, message);
    }

    // warn
    template <typename... Args>
    void warn(std::format_string<Args...> format, Args&&... args)
    {
        log(LOG_SRC_EMPTY, LogLevel::WARN, format, std::forward<Args>(args)...);
    }

    template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void warn(const T& message)
    {
        log(LOG_SRC_EMPTY, LogLevel::WARN, message);
    }

    // error
    template <typename... Args>
    void error(std::format_string<Args...> format, Args&&... args)
    {
        log(LOG_SRC_EMPTY, LogLevel::ERROR, format, std::forward<Args>(args)...);
    }

    template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void error(const T& message)
    {
        error(LOG_SRC_EMPTY, LogLevel::ERROR, message);
    }

    // fatal
    template <typename... Args>
    void fatal(std::format_string<Args...> format, Args&&... args)
    {
        log(LOG_SRC_EMPTY, LogLevel::FATAL, format, std::forward<Args>(args)...);
    }

    template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void fatal(const T& message)
    {
        log(LOG_SRC_EMPTY, LogLevel::FATAL, message);
    }

protected:
    virtual void log_it(const details::LogMsg& logMsg);
    virtual void flush_it();

    void sinks_log_it(const details::LogMsg& logMsg);
    void sinks_flush_it();

    std::vector<std::shared_ptr<Sink>> _sinkList;

private:
    std::atomic<LogLevel> _logLevel = LogLevel::INFO;
    std::string _name{"Default"};
    bool _autoFlush = false;
    std::atomic<LogLevel> _flushLevel = LogLevel::OFF;
};
}  // namespace logging

#endif  // LOGGING_LOGGER_H
