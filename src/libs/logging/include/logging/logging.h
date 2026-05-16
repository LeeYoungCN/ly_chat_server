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
void set_formatter(const std::unique_ptr<Formatter>& formatter);
void flush();

template <typename... Args>
void log(LogLevel level, std::format_string<Args...> format, Args&&... args)
{
    root_logger()->log(level, std::format(format, std::forward<Args>(args)...));
}

template <typename... Args>
void log(const logging::details::LogSource& source, LogLevel level,
         std::format_string<Args...> format, Args&&... args)
{
    root_logger()->log(source, level, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void log(const logging::details::LogSource& source, LogLevel level, const T& msg)
{
    root_logger()->log(source, level, msg);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void log(LogLevel level, const T& msg)
{
    root_logger()->log(level, utils::string::type_to_string(msg));
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void debug(const logging::details::LogSource& source, LogLevel level, const T& message)
{
    root_logger()->debug(source, level, message);
}

template <typename... Args>
void debug(std::format_string<Args...> format, Args&&... args)
{
    root_logger()->debug(format, std::forward<Args>(args)...);
}

template <typename... Args>
void debug(const logging::details::LogSource& source, std::format_string<Args...> format,
           Args&&... args)
{
    root_logger()->debug(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void debug(const T& message)
{
    root_logger()->debug(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void debug(const logging::details::LogSource& source, const T& message)
{
    root_logger()->debug(source, message);
}

template <typename... Args>
void info(std::format_string<Args...> format, Args&&... args)
{
    root_logger()->info(format, std::forward<Args>(args)...);
}

template <typename... Args>
void info(const logging::details::LogSource& source, std::format_string<Args...> format,
          Args&&... args)
{
    root_logger()->info(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void info(const T& message)
{
    root_logger()->info(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void info(const logging::details::LogSource& source, const T& message)
{
    root_logger()->info(source, message);
}

template <typename... Args>
void warn(std::format_string<Args...> format, Args&&... args)
{
    root_logger()->warn(format, std::forward<Args>(args)...);
}

template <typename... Args>
void warn(const logging::details::LogSource& source, std::format_string<Args...> format,
          Args&&... args)
{
    root_logger()->warn(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void warn(const T& message)
{
    root_logger()->warn(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void warn(const logging::details::LogSource& source, const T& message)
{
    root_logger()->warn(source, message);
}

template <typename... Args>
void error(std::format_string<Args...> format, Args&&... args)
{
    root_logger()->error(format, std::forward<Args>(args)...);
}

template <typename... Args>
void error(const logging::details::LogSource& source, std::format_string<Args...> format,
           Args&&... args)
{
    root_logger()->error(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void error(const T& message)
{
    root_logger()->error(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void error(const logging::details::LogSource& source, const T& message)
{
    root_logger()->error(source, message);
}

template <typename... Args>
void fatal(std::format_string<Args...> format, Args&&... args)
{
    root_logger()->fatal(format, std::forward<Args>(args)...);
}

template <typename... Args>
void fatal(const logging::details::LogSource& source, std::format_string<Args...> format,
           Args&&... args)
{
    root_logger()->fatal(source, format, std::forward<Args>(args)...);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void fatal(const T& message)
{
    root_logger()->fatal(message);
}

template <class T, std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
void fatal(const logging::details::LogSource& source, const T& message)
{
    root_logger()->fatal(source, message);
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
