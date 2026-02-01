#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <atomic>
#include <cstdarg>
#include <format>
#include <memory>

#include <string_view>
#include <vector>

#include "common/types/logging_types.h"
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

    void log(details::LogSource source, LogLevel level, std::string message);

    void flush();

    // template <typename... Args>
    // void log(const char* file, int line, const char* func, LogLevel level, std::format_string<Args...> fmt,
    //          Args&&... args)
    // {
    //     log(details::LogSource(file, line, func), level, std::format(fmt, std::forward<Args>(args)...));
    // }

    // template <typename... Args>
    // void log(LogLevel level, std::format_string<Args...> fmt, Args&&... args)
    // {
    //     log(details::LogSource(), level, std::format(fmt, std::forward<Args>(args)...));
    // }

    template <typename... Args>
    void debug(details::LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_DEBUG, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), LogLevel::LOG_LVL_DEBUG, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void info(details::LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_INFO, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), LogLevel::LOG_LVL_INFO, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void warn(details::LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_WARN, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void warn(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), LogLevel::LOG_LVL_WARN, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void error(details::LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_ERR, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), LogLevel::LOG_LVL_ERR, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void fatal(details::LogSource logSrc, std::format_string<Args...> fmt, Args&&... args)
    {
        log(logSrc, LogLevel::LOG_LVL_FATAL, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void fatal(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), LogLevel::LOG_LVL_FATAL, std::format(fmt, std::forward<Args>(args)...));
    }

protected:
    virtual void log_it(details::LogMsg&& logMsg);
    virtual void flush_it();

    void sinks_log_it();
    void sinks_flush_it();

    std::vector<std::shared_ptr<Sink>> _sinkList;
private:
    std::atomic<LogLevel> _logLevel = LogLevel::LOG_LVL_INFO;
    std::string _name{"Default"};
    bool _autoFlush = false;
    std::atomic<LogLevel> _flushLevel = LogLevel::LOG_LVL_FATAL;
};
}  // namespace logging

#endif  // LOGGING_LOGGER_H
