#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <atomic>
#include <cstdarg>
#include <format>
#include <memory>
#include <string_view>
#include <vector>

#include "common/types/logging_types.h"
#include "logging/log_source.h"
#include "logging/sinks/sink.h"

namespace logging {
class Logger {
public:
    Logger() = default;
    explicit Logger(std::string_view name);
    ~Logger();

    void set_level(LogLevel level);
    void set_name(std::string_view name);
    std::string_view get_name() const;

    bool should_log(LogLevel level) const;

    void add_sink(const std::shared_ptr<Sink>& sink);

    void log(LogSource source, LogLevel level, std::string message);

    void flush();

    // template <typename... Args>
    // void log(const char* file, int line, const char* func, LogLevel level, std::format_string<Args...> fmt,
    //          Args&&... args)
    // {
    //     log(LogSource(file, line, func), level, std::format(fmt, std::forward<Args>(args)...));
    // }

    // template <typename... Args>
    // void log(LogLevel level, std::format_string<Args...> fmt, Args&&... args)
    // {
    //     log(LogSource(), level, std::format(fmt, std::forward<Args>(args)...));
    // }

    template <typename... Args>
    void debug(LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_DEBUG, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args)
    {
        log(LogSource(), LogLevel::LOG_LVL_DEBUG, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void info(LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_INFO, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args)
    {
        log(LogSource(), LogLevel::LOG_LVL_INFO, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void warn(LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_WARN, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void warn(std::format_string<Args...> fmt, Args&&... args)
    {
        log(LogSource(), LogLevel::LOG_LVL_WARN, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void error(LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_ERR, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args)
    {
        log(LogSource(), LogLevel::LOG_LVL_ERR, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void fatal(LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_FATAL, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void fatal(std::format_string<Args...> fmt, Args&&... args)
    {
        log(LogSource(), LogLevel::LOG_LVL_FATAL, std::format(fmt, std::forward<Args>(args)...));
    }

private:
    std::atomic<LogLevel> m_level = LogLevel::LOG_LVL_INFO;
    std::vector<std::shared_ptr<Sink>> m_sinkList;
    std::string m_name{"Default"};
};
}  // namespace logging

#endif  // LOGGING_LOGGER_H
