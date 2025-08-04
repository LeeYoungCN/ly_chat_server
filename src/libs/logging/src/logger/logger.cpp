#include "logging/logger.h"

#include <algorithm>
#include <cstdarg>
#include <mutex>
#include <thread>
#include <vector>

#include "common/common_macros.h"
#include "common/utils/date_time_utils.h"
#include "internal/console_appender.h"
#include "logging/appender.h"
#include "logging/logging_types.h"

namespace logging {
Logger::Logger(std::string name) : m_name(std::move(name)) {};

Logger::Logger(std::string name, common::types::logging::LogLevel level)
    : m_name(std::move(name)), m_logLevel(level) {};

const std::string& Logger::getLoggerName()
{
    return m_name;
}

void Logger::addAppender(const std::shared_ptr<Appender>& appender)
{
    std::unique_lock<std::mutex> lock(m_appenderMtx);
    m_appenderList.emplace_back(appender);
}

void Logger::setLogLevel(common::types::logging::LogLevel level)
{
    m_logLevel = level;
}

void Logger::log(common::types::logging::LogLevel level, const char* file, int32_t line, const char* func,
                 const char* fmt, ...)
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
                        .time = common::utils::date_time::GetCurrentTimestamp(),
                        .file = file,
                        .line = line,
                        .func = func,
                        .threadId = std::this_thread::get_id(),
                        .loggerName = getLoggerName(),
                        .message = buffer};
    {
        std::unique_lock<std::mutex> lock(m_appenderMtx);
        if (m_appenderList.empty()) {
            INST(ConsoleAppender).append(record);
        } else {
            for (const auto& appender : m_appenderList) {
                appender->append(record);
            }
        }
    }
}

void Logger::removeAppender(const std::shared_ptr<Appender>& appender)
{
    std::lock_guard<std::mutex> lock(m_appenderMtx);
    auto it = std::remove(m_appenderList.begin(), m_appenderList.end(), appender);
    m_appenderList.erase(it, m_appenderList.end());
}

void Logger::removeAllAppender()
{
    std::lock_guard<std::mutex> lock(m_appenderMtx);
    m_appenderList.clear();
}
}  // namespace logging
