#ifndef LOGGING_LOGGERS_LOGGER_H
#define LOGGING_LOGGERS_LOGGER_H

#include <format>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

#include "common/types/type_traits.h"
#include "logging/details/log_source.h"
#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/sinks/sink.h"
#include "utils/string_utils.h"

namespace logging {
class Logger {
public:
    Logger() = default;
    virtual ~Logger() = default;

    [[nodiscard]] virtual std::string_view name() const = 0;
    [[nodiscard]] virtual std::vector<std::shared_ptr<logging::Sink>> sinks() const = 0;

    void virtual set_level(LogLevel level) const = 0;
    [[nodiscard]] virtual LogLevel level() const = 0;
    [[nodiscard]] virtual bool should_log(LogLevel level) const = 0;

    virtual void flush_on(LogLevel level) const = 0;
    [[nodiscard]] virtual LogLevel flush_level() const = 0;
    [[nodiscard]] virtual bool should_flush(LogLevel level) const = 0;

    virtual void set_pattern(std::string_view pattern) const = 0;
    virtual void set_formatter(const std::unique_ptr<logging::Formatter>& formatter) const = 0;

    virtual void flush() = 0;

#pragma region log function
    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void log(const details::LogSource& source, LogLevel level, const T& message)
    {
        if (should_log(level)) {
            log_it(source, level, utils::string::type_to_string(message));
        }
    }

    template <typename... Args>
    void log(const details::LogSource& source, LogLevel level, std::format_string<Args...> format,
             Args&&... args)
    {
        if (should_log(level)) {
            log_it(source, level, std::format(format, std::forward<Args>(args)...));
        }
    }

    template <typename... Args>
    void trace(const details::LogSource& source, std::format_string<Args...> format, Args&&... args)
    {
        log(source, logging::LogLevel::TRACE, format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(const details::LogSource& source, std::format_string<Args...> format, Args&&... args)
    {
        log(source, logging::LogLevel::DEBUG, format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(const details::LogSource& source, std::format_string<Args...> format, Args&&... args)
    {
        log(source, logging::LogLevel::INFO, format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(const details::LogSource& source, std::format_string<Args...> format, Args&&... args)
    {
        log(source, logging::LogLevel::WARN, format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(const details::LogSource& source, std::format_string<Args...> format, Args&&... args)
    {
        log(source, logging::LogLevel::ERR, format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void fatal(const details::LogSource& source, std::format_string<Args...> format, Args&&... args)
    {
        log(source, logging::LogLevel::FATAL, format, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void trace(std::format_string<Args...> format, Args&&... args)
    {
        log(logging::details::LogSource(),
            logging::LogLevel::TRACE,
            format,
            std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(std::format_string<Args...> format, Args&&... args)
    {
        log(logging::details::LogSource(),
            logging::LogLevel::DEBUG,
            format,
            std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(std::format_string<Args...> format, Args&&... args)
    {
        log(logging::details::LogSource(),
            logging::LogLevel::INFO,
            format,
            std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(std::format_string<Args...> format, Args&&... args)
    {
        log(logging::details::LogSource(),
            logging::LogLevel::WARN,
            format,
            std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(std::format_string<Args...> format, Args&&... args)
    {
        log(logging::details::LogSource(),
            logging::LogLevel::ERR,
            format,
            std::forward<Args>(args)...);
    }

    template <typename... Args>
    void fatal(std::format_string<Args...> format, Args&&... args)
    {
        log(logging::details::LogSource(),
            logging::LogLevel::FATAL,
            format,
            std::forward<Args>(args)...);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void trace(const T& message)
    {
        log(logging::details::LogSource(), logging::LogLevel::TRACE, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void debug(const T& message)
    {
        log(logging::details::LogSource(), logging::LogLevel::DEBUG, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void info(const T& message)
    {
        log(logging::details::LogSource(), logging::LogLevel::INFO, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void warn(const T& message)
    {
        log(logging::details::LogSource(), logging::LogLevel::WARN, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void error(const T& message)
    {
        log(logging::details::LogSource(), logging::LogLevel::ERR, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void fatal(const T& message)
    {
        log(logging::details::LogSource(), logging::LogLevel::FATAL, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void trace(const details::LogSource& source, const T& message)
    {
        log(source, logging::LogLevel::TRACE, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void debug(const details::LogSource& source, const T& message)
    {
        log(source, logging::LogLevel::DEBUG, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void info(const details::LogSource& source, const T& message)
    {
        log(source, logging::LogLevel::INFO, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void warn(const details::LogSource& source, const T& message)
    {
        log(source, logging::LogLevel::WARN, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void error(const details::LogSource& source, const T& message)
    {
        log(source, logging::LogLevel::ERR, message);
    }

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void fatal(const details::LogSource& source, const T& message)
    {
        log(source, logging::LogLevel::FATAL, message);
    }
#pragma endregion

protected:
    virtual void log_it(const details::LogSource& source, LogLevel level,
                        std::string_view message) = 0;
};
}  // namespace logging

#endif  // LOGGING_LOGGERS_LOGGER_H
