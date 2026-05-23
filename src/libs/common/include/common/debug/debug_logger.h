#ifndef COMMON_DEBUG_DEBUG_LOGGER_H
#define COMMON_DEBUG_DEBUG_LOGGER_H

#include <cstdarg>
#include <format>

#include "common/debug/debug_level.h"

namespace common::debug {

void set_common_debug_logger_level(DebugLevel level);

void common_debug_logger_log(const char* file, int line, const char* func, DebugLevel level,
                             const std::string& message);

template <typename... Args>
void common_debug_logger_log(const char* file, int line, const char* func, DebugLevel level,
                             std::format_string<Args...> format, Args&&... args)
{
    common_debug_logger_log(
        file, line, func, level, std::move(std::format(format, std::forward<Args>(args)...)));
}

}  // namespace common::debug

#if defined(NDEBUG) && !defined(ENABLE_TEST)
// Release模式：空操作，显式消费所有参数避免警告
#define DEBUG_LOGGER(level, format, ...) (static_cast<void>(0))
#else
// Debug模式：实际日志输出，支持所有级别
#define DEBUG_LOGGER(level, format, ...)                                                 \
    do {                                                                                 \
        common::debug::common_debug_logger_log(                                          \
            __FILE__, __LINE__, __FUNCTION__, level, format __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)
#endif

#define DEBUG_LOGGER_DBG(format, ...) \
    DEBUG_LOGGER(DEBUG_LVL_DEBUG, format __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOGGER_INFO(format, ...) \
    DEBUG_LOGGER(DEBUG_LVL_INFO, format __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOGGER_WARN(format, ...) \
    DEBUG_LOGGER(DEBUG_LVL_WARN, format __VA_OPT__(, ) __VA_ARGS__);

#define DEBUG_LOGGER_ERR(format, ...) \
    DEBUG_LOGGER(DEBUG_LVL_ERR, format __VA_OPT__(, ) __VA_ARGS__);

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

#define DEBUG_LOGGER_EXCEPTION(e, format, ...)                       \
    do {                                                             \
        DEBUG_LOGGER_ERR(format " excp: {}" #__VA_ARGS__, e.what()); \
    } while (0)

#endif  // COMMON_DEBUG_DEBUG_LOG_HPP
