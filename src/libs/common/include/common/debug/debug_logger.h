#ifndef COMMON_DEBUG_DEBUG_LOGGER_H
#define COMMON_DEBUG_DEBUG_LOGGER_H

#include <cstdarg>
#include <format>

#include "common/base/singleton.h"
#include "common/debug/debug_level.h"

namespace common::debug {

class DebugLogger : public base::SingletonBase<DebugLogger> {
    friend class base::SingletonBase<DebugLogger>;

public:
    void set_debug_log_level(DebugLevel level);

    bool should_log(DebugLevel level);

    void log_va(DebugLevel level, const char* file, int line, const char* func, const char* format, va_list args);

    template <typename... Args>
    void log(DebugLevel level, std::format_string<Args...> format, Args&&... args)
    {
        log_it(level, std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void log(DebugLevel level, const char* file, int line, const char* func, std::format_string<Args...> format,
             Args&&... args)
    {
        log_it(level, std::move(std::format(format, std::forward<Args>(args)...)), file, line, func);
    }

    template <typename... Args>
    void debug(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_DEBUG, std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void debug(const char* file, int line, const char* func, std::format_string<Args...> format, Args&&... args)
    {
        log_it(
            DebugLevel::DEBUG_LVL_DEBUG, std::move(std::format(format, std::forward<Args>(args)...)), file, line, func);
    }

    template <typename... Args>
    void info(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_INFO, std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void info(const char* file, int line, const char* func, std::format_string<Args...> format, Args&&... args)
    {
        log_it(
            DebugLevel::DEBUG_LVL_INFO, std::move(std::format(format, std::forward<Args>(args)...)), file, line, func);
    }

    template <typename... Args>
    void warn(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_WARN, std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void warn(const char* file, int line, const char* func, std::format_string<Args...> format, Args&&... args)
    {
        log_it(
            DebugLevel::DEBUG_LVL_WARN, std::move(std::format(format, std::forward<Args>(args)...)), file, line, func);
    }

    template <typename... Args>
    void error(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_ERR, std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void error(const char* file, int line, const char* func, std::format_string<Args...> format, Args&&... args)
    {
        log_it(
            DebugLevel::DEBUG_LVL_ERR, std::move(std::format(format, std::forward<Args>(args)...)), file, line, func);
    }

    template <typename... Args>
    void fatal(std::format_string<Args...> format, Args&&... args)
    {
        log_it(DebugLevel::DEBUG_LVL_FATAL, std::move(std::format(format, std::forward<Args>(args)...)));
    }

    template <typename... Args>
    void fatal(const char* file, int line, const char* func, std::format_string<Args...> format, Args&&... args)
    {
        log_it(
            DebugLevel::DEBUG_LVL_FATAL, std::move(std::format(format, std::forward<Args>(args)...)), file, line, func);
    }

private:
    DebugLogger() = default;
    ~DebugLogger() override = default;

private:
    void log_it(DebugLevel level, std::string&& message, const char* file = nullptr, int line = 0,
                const char* func = nullptr);
    std::string format_log(DebugLevel level, std::string&& message, const char* file = nullptr, int line = 0,
                           const char* func = nullptr);

    size_t get_current_tid();
    std::string time_string();
    std::string va_list_to_string(const char* format, va_list args);

private:
    DebugLevel _logLevel = DebugLevel::DEBUG_LVL_INFO;
};

}  // namespace common::debug

#if defined(NDEBUG) && !defined(ENABLE_TEST)
// Release模式：空操作，显式消费所有参数避免警告
#define DEBUG_LOGGER(level, format, ...) (static_cast<void>(0))
#else
// Debug模式：实际日志输出，支持所有级别
#define DEBUG_LOGGER(level, format, ...)                                                 \
    do {                                                                                 \
        common::debug::DebugLogger::Instance().log(                                      \
            level, __FILE__, __LINE__, __FUNCTION__, format __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)
#endif

#define DEBUG_LOGGER_DBG(format, ...) DEBUG_LOGGER(DEBUG_LVL_DEBUG, format __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOGGER_INFO(format, ...) DEBUG_LOGGER(DEBUG_LVL_INFO, format __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOGGER_WARN(format, ...) DEBUG_LOGGER(DEBUG_LVL_WARN, format __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOGGER_ERR(format, ...) DEBUG_LOGGER(DEBUG_LVL_ERR, format __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOGGER_FATAL(format, ...)                                   \
    do {                                                                  \
        DEBUG_LOGGER(DEBUG_LVL_FATAL, format __VA_OPT__(, ) __VA_ARGS__); \
        std::abort();                                                     \
    } while (0)

#define DEBUG_LOGGER_COND(condition, format, ...)                            \
    do {                                                                     \
        if (!(condition)) {                                                  \
            DEBUG_LOGGER_ERR("[FAILED] " format __VA_OPT__(, ) __VA_ARGS__); \
        } else {                                                             \
            DEBUG_LOGGER_DBG("[SUCCESS] " format __VA_OPT__(, ) __VA_ARGS__) \
        }                                                                    \
    } while (0)

#define DEBUG_LOGGER_EXCEPTION(e, format, ...)                                     \
    do {                                                                           \
        DEBUG_LOGGER_ERR(format " excp: {}" __VA_OPT__(, ) __VA_ARGS__, e.what()); \
    } while (0)

#endif  // COMMON_DEBUG_DEBUG_LOG_HPP
