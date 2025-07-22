/**
 * @file logger_factory.cpp
 * @author your name (you@domain.com)
 * @brief 日志器工厂
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "logging/logger_factory.h"

#include <memory>
#include <mutex>

namespace logging {

std::shared_ptr<Logger> LoggerFactory::getLogger(const std::string& loggerName)
{
    std::unique_lock<std::mutex> lock(m_mapMutex);
    if (m_loggerMap.find(loggerName) == m_loggerMap.end()) {
        m_loggerMap[loggerName] = std::make_shared<Logger>(loggerName);
    }
    return m_loggerMap[loggerName];
}

void LoggerFactory::registerLogger(const std::string& loggerName, const std::shared_ptr<Logger>& logger)
{
    std::unique_lock<std::mutex> lock(m_mapMutex);
    m_loggerMap[loggerName] = logger;
}

void LoggerFactory::removeLogger(const std::string& loggerName)
{
    std::unique_lock<std::mutex> lock(m_mapMutex);
    m_loggerMap.erase(loggerName);
}
}  // namespace logging
