#pragma once
#ifndef LOGGING_DETAILS_COMMON_H
#define LOGGING_DETAILS_COMMON_H

#include <cstdint>

#define DEFINE_LOGGER_LOG_FUNCTION(Name, Level)                                                 \
    template <typename... Args>                                                                 \
    void Name(std::format_string<Args...> format, Args&&... args)                               \
    {                                                                                           \
        log(LOG_SRC_EMPTY, Level, format, std::forward<Args>(args)...);                         \
    }                                                                                           \
                                                                                                \
    template <typename... Args>                                                                 \
    void Name(                                                                                  \
        logging::details::LogSource source, std::format_string<Args...> format, Args&&... args) \
    {                                                                                           \
        log(source, Level, format, std::forward<Args>(args)...);                                \
    }                                                                                           \
                                                                                                \
    template <class T,                                                                          \
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>    \
    void Name(const T& message)                                                                 \
    {                                                                                           \
        log(LOG_SRC_EMPTY, Level, message);                                                     \
    }                                                                                           \
                                                                                                \
    template <class T,                                                                          \
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>    \
    void Name(logging::details::LogSource source, const T& message)                             \
    {                                                                                           \
        log(source, Level, message);                                                            \
    }

#define LOG_SRC_LOCAL logging::details::LogSource(__FILE__, __LINE__, __FUNCTION__)
#define LOG_SRC_EMPTY logging::details::LogSource()

namespace logging {
constexpr const char* DEFAULT_PATTERN = "[%d][%L][%s:%#]: %v";
constexpr const char* DEFAULT_TIME_PATTERN = "%Y-%m-%d %H:%M:%S.%3f";
constexpr const char* DEFAULT_LOGGER_NAME = "__default_logger__";

constexpr uint32_t LOG_CONTENT_DEFAULT_LEN = 256;
}  // namespace logging

#endif  // LOGGING_DETAILS_COMMON_H
