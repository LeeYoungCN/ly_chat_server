#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "common/types/log_level.h"
#include "logging/appender.h"

namespace logging {
class Logger {
public:
    Logger() = default;
    ~Logger() = default;
    explicit Logger(std::string name);
    Logger(std::string name, common::types::LogLevel level);
    void log(common::types::LogLevel level, const char* file, int32_t line, const char* func, const char* fmt, ...);

    const std::string& getLoggerName();
    void setLogLevel(common::types::LogLevel level);
    void addAppder(const std::shared_ptr<Appender>& apender);
    void removeAppender(const std::shared_ptr<Appender>& appender);
    void removeAllAppender();

private:
    std::string m_name = "root";
    common::types::LogLevel m_logLevel = common::types::LogLevel::INFO;
    std::vector<std::shared_ptr<Appender>> m_appenders;
    std::mutex m_apenderMtx;
};
}  // namespace logging
#endif  // LOGGER_H
