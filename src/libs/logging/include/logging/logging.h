#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#include <format>
#include <memory>
#include <string_view>
#include <utility>

#include "logging/details/log_source.h"
#include "logging/details/task_pool.h"
#include "logging/log_level.h"
#include "logging/logger.h"

namespace logging {

template <typename SinkType, typename... SinkArgs>
std::shared_ptr<Logger> create_logger(std::string name, SinkArgs&&... sinkArgs)
{
    auto sink = std::make_shared<SinkType>(std::forward<SinkArgs>(sinkArgs)...);
    return std::make_shared<Logger>(std::move(name), std::move(sink));
}

#pragma region Root logger
std::shared_ptr<Logger> root_logger();
Logger* root_logger_raw();
void set_root_logger(std::shared_ptr<Logger> logger);

bool should_log(LogLevel level);
void set_level(LogLevel level);
void flush_on(LogLevel level);
void set_pattern(std::string_view pattern = FORMATTER_DEFAULT_PATTERN,
                 std::string_view timePattern = FORMATTER_DEFAULT_TIME_PATTERN);
void set_formatter(std::unique_ptr<Formatter> formatter);
void flush();

template <typename... Args>
void log(LogLevel level, std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->log(level, format, std::forward<Args>(args)...);
}

template <typename... Args>
void log(logging::details::LogSource source, LogLevel level, std::format_string<Args...> format,
         Args&&... args)
{
    root_logger_raw()->log(source, level, format, std::forward<Args>(args)...);
}

template <typename T>
void log(logging::details::LogSource source, LogLevel level, const T& msg)
{
    root_logger_raw()->log(source, level, msg);
}

template <typename T>
void log(LogLevel level, const T& msg)
{
    root_logger_raw()->log(level, msg);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void log(LogLevel level, const T& message)
{
    root_logger_raw()->log(level, message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void debug(logging::details::LogSource source, LogLevel level, const T& message)
{
    root_logger_raw()->log(source, level, message);
}

template <typename... Args>
void debug(std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->debug(format, std::forward<Args>(args)...);
}

template <typename... Args>
void debug(logging::details::LogSource source, std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->debug(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void debug(const T& message)
{
    root_logger_raw()->debug(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void debug(logging::details::LogSource source, const T& message)
{
    root_logger_raw()->debug(source, message);
}

template <typename... Args>
void info(std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->info(format, std::forward<Args>(args)...);
}

template <typename... Args>
void info(logging::details::LogSource source, std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->info(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void info(const T& message)
{
    root_logger_raw()->info(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void info(logging::details::LogSource source, const T& message)
{
    root_logger_raw()->info(source, message);
}

template <typename... Args>
void warn(std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->warn(format, std::forward<Args>(args)...);
}

template <typename... Args>
void warn(logging::details::LogSource source, std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->warn(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void warn(const T& message)
{
    root_logger_raw()->warn(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void warn(logging::details::LogSource source, const T& message)
{
    root_logger_raw()->warn(source, message);
}

template <typename... Args>
void error(std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->error(format, std::forward<Args>(args)...);
}

template <typename... Args>
void error(logging::details::LogSource source, std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->error(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void error(const T& message)
{
    root_logger_raw()->error(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void error(logging::details::LogSource source, const T& message)
{
    root_logger_raw()->error(source, message);
}

template <typename... Args>
void fatal(std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->fatal(format, std::forward<Args>(args)...);
}

template <typename... Args>
void fatal(logging::details::LogSource source, std::format_string<Args...> format, Args&&... args)
{
    root_logger_raw()->fatal(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void fatal(const T& message)
{
    root_logger_raw()->fatal(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void fatal(logging::details::LogSource source, const T& message)
{
    root_logger_raw()->fatal(source, message);
}
#pragma endregion

#pragma region Module manager
void initialize_logger(const std::shared_ptr<Logger>& logger);
void set_level_all(LogLevel level);
void flush_on_all(LogLevel level);
void set_pattern_all(std::string_view pattern = FORMATTER_DEFAULT_PATTERN,
                     std::string_view timePattern = FORMATTER_DEFAULT_TIME_PATTERN);
void set_formatter_all(std::unique_ptr<Formatter> formatter);
void flush_all();
void shut_down();
#pragma endregion

#pragma region Container
bool register_logger(std::shared_ptr<Logger> logger);
void register_or_replace_logger(std::shared_ptr<Logger> logger);
void remove_logger(std::string_view name);
void remove_all();
std::shared_ptr<Logger> get_logger(std::string_view name);
void register_task_pool(std::shared_ptr<details::TaskPool> taskPool);
std::shared_ptr<details::TaskPool> get_task_pool();
#pragma endregion

}  // namespace logging

#endif  // LOGGING_LOGGING_H
