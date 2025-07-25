#include "logging/logger.h"

#include <algorithm>
#include <cstdarg>
#include <mutex>
#include <thread>
#include <vector>

#include "common/macros.h"
#include "logging/appender.h"
#include "internal/console_appender.h"
#include "logging/logging_types.h"
#include "utils/time_utils.h"

namespace logging {
Logger::Logger(std::string name) : m_name(std::move(name)) {};

Logger::Logger(std::string name, common::types::LogLevel level) : m_name(std::move(name)), m_logLevel(level) {};

const std::string& Logger::getLoggerName()
{
    return m_name;
}

void Logger::addAppder(const std::shared_ptr<Appender>& apender)
{
    std::unique_lock<std::mutex> lock(m_apenderMtx);
    m_appenders.emplace_back(apender);
}

void Logger::setLogLevel(common::types::LogLevel level)
{
    m_logLevel = level;
}

void Logger::log(common::types::LogLevel level, const char* file, int32_t line, const char* func, const char* fmt, ...)
{
    if (level < m_logLevel) {
        return;
    }
    constexpr uint32_t BUFFER_LEN = 512;
    char buffer[BUFFER_LEN] = {'\0'};
    va_list argList;
    va_start(argList, fmt);
    vsnprintf(buffer, BUFFER_LEN, fmt, argList);
    va_end(argList);

    LogRecord record = {.level = level,
                        .time = utils::time::GetCurrentTimeStamp(),
                        .file = file,
                        .line = line,
                        .func = func,
                        .threadId = std::this_thread::get_id(),
                        .loggerName = getLoggerName(),
                        .message = buffer};
    {
        std::unique_lock<std::mutex> lock(m_apenderMtx);
        if (m_appenders.empty()) {
            INST(ConsoleAppender).append(record);
        } else {
            for (const auto& apender : m_appenders) {
                apender->append(record);
            }
        }
    }
}

void Logger::removeAppender(const std::shared_ptr<Appender>& appender)
{
    std::lock_guard<std::mutex> lock(m_apenderMtx);
    auto it = std::remove(m_appenders.begin(), m_appenders.end(), appender);
    m_appenders.erase(it, m_appenders.end());
}

void Logger::removeAllAppender()
{
    std::lock_guard<std::mutex> lock(m_apenderMtx);
    m_appenders.clear();
}
}  // namespace logging
