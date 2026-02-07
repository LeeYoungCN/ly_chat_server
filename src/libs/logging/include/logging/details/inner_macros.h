#pragma once
#ifndef LOGGING_DETAIL_INNER_MACROS_H
#define LOGGING_DETAIL_INNER_MACROS_H

/**
 * @brief logging.h
 *
 */
#define DEFINE_LOGGING_LOG_FUNCTION(Name)                                                       \
    template <typename... Args>                                                                 \
    void Name(std::format_string<Args...> format, Args&&... args)                               \
    {                                                                                           \
        root_logger_raw()->Name(format, std::forward<Args>(args)...);                           \
    }                                                                                           \
                                                                                                \
    template <typename... Args>                                                                 \
    void Name(                                                                                  \
        logging::details::LogSource source, std::format_string<Args...> format, Args&&... args) \
    {                                                                                           \
        root_logger_raw()->Name(source, format, std::forward<Args>(args)...);                   \
    }                                                                                           \
                                                                                                \
    template <class T,                                                                          \
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>    \
    void Name(const T& message)                                                                 \
    {                                                                                           \
        root_logger_raw()->Name(message);                                                       \
    }                                                                                           \
                                                                                                \
    template <class T,                                                                          \
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>    \
    void Name(logging::details::LogSource source, const T& message)                             \
    {                                                                                           \
        root_logger_raw()->Name(source, message);                                               \
    }

/**
 * @brief logger.h
 *
 */
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

#endif  // LOGGING_DETAIL_INNER_MACROS_H
