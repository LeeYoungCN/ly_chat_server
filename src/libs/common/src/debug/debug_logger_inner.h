#ifndef COMMON_DEBUG_DEBUG_LOGGER_INNER_H
#define COMMON_DEBUG_DEBUG_LOGGER_INNER_H

#include <cstdarg>
#include <format>
#include <mutex>
#include <string>

#include "common/base/singleton.h"
#include "common/debug/debug_level.h"
#include "common/types/type_traits.h"

namespace common::debug {

class DebugLoggerInner : public base::SingletonBase<DebugLoggerInner> {
    friend class base::SingletonBase<DebugLoggerInner>;

public:
    void set_debug_log_level(DebugLevel level);

    bool should_log(DebugLevel level);

    void log_va(const char* file, int line, const char* func, DebugLevel level, const char* format,
                va_list args);

    template <class T, std::enable_if_t<type_traits::is_convertible_to_string_v<T>, int> = 0>
    void log(DebugLevel level, const T& message)
    {
        log_it(level, std::move(type_to_string(message)));
    }

    template <typename T, std::enable_if_t<type_traits::is_convertible_to_string_v<T>, int> = 0>
    void log(const char* file, int line, const char* func, DebugLevel level, const T& message)
    {
        log_it(level, std::move(type_to_string(message)), file, line, func);
    }

    template <typename... Args>
    void log(DebugLevel level, std::format_string<Args...> format, Args&&... args)
    {
        log_it(level, std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void log(const char* file, int line, const char* func, DebugLevel level,
             std::format_string<Args...> format, Args&&... args)
    {
        log_it(
            level, std::move(std::format(format, std::forward<Args>(args)...)), file, line, func);
    }

    template <class T>
    void debug(const T& message)
    {
        log_it(DebugLevel::DEBUG_LVL_DEBUG, std::move(type_to_string(message)));
    }

    template <typename... Args>
    void debug(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_DEBUG,
               std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <class T>
    void info(const T& message)
    {
        log_it(DebugLevel::DEBUG_LVL_INFO, std::move(std::format(type_to_string(message))));
    }

    template <typename... Args>
    void info(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_INFO,
               std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void info(const char* file, int line, const char* func, std::format_string<Args...> format,
              Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_INFO,
               std::move(std::format(format, std::forward<Args>(args)...)),
               file,
               line,
               func);
    }

    template <typename... Args>
    void warn(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_WARN,
               std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void warn(const char* file, int line, const char* func, std::format_string<Args...> format,
              Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_WARN,
               std::move(std::format(format, std::forward<Args>(args)...)),
               file,
               line,
               func);
    }

    template <typename... Args>
    void error(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_ERR,
               std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void error(const char* file, int line, const char* func, std::format_string<Args...> format,
               Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_ERR,
               std::move(std::format(format, std::forward<Args>(args)...)),
               file,
               line,
               func);
    }

    template <typename... Args>
    void fatal(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_FATAL,
               std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void fatal(const char* file, int line, const char* func, std::format_string<Args...> format,
               Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_FATAL,
               std::move(std::format(format, std::forward<Args>(args)...)),
               file,
               line,
               func);
    }

private:
    DebugLoggerInner() = default;
    ~DebugLoggerInner() override = default;

private:
    void log_it(DebugLevel level, const std::string& message, const char* file = nullptr,
                int line = 0, const char* func = nullptr);
    std::string format_log(DebugLevel level, const std::string& message, const char* file = nullptr,
                           int line = 0, const char* func = nullptr);

    size_t get_current_tid();
    std::string va_list_to_string(const char* format, va_list args);

    template <class T, std::enable_if_t<type_traits::is_convertible_to_string_v<T>, int> = 0>
    std::string type_to_string(const T& message)
    {
        std::string str;
        if constexpr (type_traits::is_direct_string_type_v<T>) {
            str = std::string(message);
        } else if constexpr (type_traits::is_numeric_type_v<T>) {
            str = std::to_string(message);
        }
        return str;
    }

private:
    DebugLevel _logLevel = DebugLevel::DEBUG_LVL_INFO;
    std::mutex _mtx;
};

}  // namespace common::debug

#endif  // COMMON_DEBUG_DEBUG_LOG_HPP
