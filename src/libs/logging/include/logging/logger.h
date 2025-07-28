#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "common/types/logging_types.h"
#include "logging/appender.h"

namespace logging {
class Logger {
public:
    Logger() = default;
    ~Logger() = default;
    explicit Logger(std::string name);
    Logger(std::string name, common::types::logging::LogLevel level);
    void log(common::types::logging::LogLevel level, const char* file, int32_t line, const char* func, const char* fmt, ...);

    const std::string& getLoggerName();
    void setLogLevel(common::types::logging::LogLevel level);
    void addAppder(const std::shared_ptr<Appender>& apender);
    void removeAppender(const std::shared_ptr<Appender>& appender);
    void removeAllAppender();

private:
    std::string m_name = "root";
    common::types::logging::LogLevel m_logLevel = common::types::logging::LogLevel::INFO;
    std::vector<std::shared_ptr<Appender>> m_appenders;
    std::mutex m_apenderMtx;
};
}  // namespace logging
#endif  // LOGGING_LOGGER_H
