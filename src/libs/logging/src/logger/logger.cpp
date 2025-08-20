#include "logging/logger.h"

#include <algorithm>
#include <cstdarg>
#include <mutex>
#include <utility>
#include <vector>

#include "logging/appender/appender.h"
#include "logging/details/log_record.h"
#include "logging/details/logging_base.h"

namespace logging {

using namespace logging::details;
using namespace common::types::logging;

Logger::Logger(std::string name) : LoggingBase(std::move(name)) {};

void Logger::addAppender(const std::shared_ptr<Appender>& appender)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    m_appenderList.emplace_back(appender);
}

void Logger::logInternal(LogSource source, LogLevel level, const std::string& message)
{
    LogRecord record(source, level, m_name, message);
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        for (const auto& appender : m_appenderList) {
            if (appender->shouldLog(level)) {
                appender->append(record);
            }
        }
    }
}
bool Logger::shouldLog(common::types::logging::LogLevel level) const
{
    return getLevel() <= level;
}

void Logger::setLevel(common::types::logging::LogLevel level)
{
    return m_level.store(level);
};

common::types::logging::LogLevel Logger::getLevel() const
{
    return m_level.load(std::memory_order_relaxed);
}

void Logger::deleteAppender(const std::shared_ptr<Appender>& appender)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = std::remove(m_appenderList.begin(), m_appenderList.end(), appender);
    m_appenderList.erase(it, m_appenderList.end());
}

void Logger::clearAppender()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_appenderList.clear();
}

}  // namespace logging
