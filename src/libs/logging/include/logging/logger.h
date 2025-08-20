#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <format>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "common/types/logging_types.h"
#include "logging/appender/appender.h"
#include "logging/details/log_source.h"
#include "logging/details/logging_base.h"

namespace logging {
class Logger : public details::LoggingBase {
public:
    Logger() = default;
    ~Logger() override = default;
    explicit Logger(std::string name);
    Logger(std::string name, common::types::logging::LogLevel level);

    template <typename... Args>
    void log(details::LogSource source, common::types::logging::LogLevel level, std::format_string<Args...> fmt,
             Args&&... args)
    {
        if (!shouldLog(level)) {
            return;
        }
        logInternal(source, level, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    void debug(details::LogSource source, std::format_string<Args...> fmt, Args&&... args)
    {
        log(source, common::types::logging::LogLevel::DEBUG, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void debug(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), common::types::logging::LogLevel::DEBUG, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(details::LogSource source, std::format_string<Args...> fmt, Args&&... args)
    {
        log(source, common::types::logging::LogLevel::INFO, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void info(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), common::types::logging::LogLevel::INFO, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(details::LogSource source, std::format_string<Args...> fmt, Args&&... args)
    {
        log(source, common::types::logging::LogLevel::WARN, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void warn(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), common::types::logging::LogLevel::WARN, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(details::LogSource source, std::format_string<Args...> fmt, Args&&... args)
    {
        log(source, common::types::logging::LogLevel::ERR, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void error(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), common::types::logging::LogLevel::ERR, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void critical(details::LogSource source, std::format_string<Args...> fmt, Args&&... args)
    {
        log(source, common::types::logging::LogLevel::FATAL, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    void critical(std::format_string<Args...> fmt, Args&&... args)
    {
        log(details::LogSource(), common::types::logging::LogLevel::FATAL, fmt, std::forward<Args>(args)...);
    }

    bool shouldLog(common::types::logging::LogLevel level) const;

    void setLevel(common::types::logging::LogLevel level);

    common::types::logging::LogLevel getLevel() const;

    void addAppender(const std::shared_ptr<Appender>& appender);
    void deleteAppender(const std::shared_ptr<Appender>& appender);
    void clearAppender();

private:
    void logInternal(details::LogSource resource, common::types::logging::LogLevel level, const std::string& message);

private:
    std::atomic<common::types::logging::LogLevel> m_level{common::types::logging::LogLevel::INFO};
    std::vector<std::shared_ptr<Appender>> m_appenderList;
    std::mutex m_mutex;
};
}  // namespace logging
#endif  // LOGGING_LOGGER_H
