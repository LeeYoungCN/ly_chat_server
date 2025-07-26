/**
 * @file logger_factory.h
 * @author your name (you@domain.com)
 * @brief 日志器工厂
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

#ifndef LOGGER_FACTORY_H
#define LOGGER_FACTORY_H

#include "common/base/singleton.h"
#include "logging/logger.h"

namespace logging {
class LoggerFactory : public common::base::SingletonBase<LoggerFactory> {
public:
    std::shared_ptr<Logger> getLogger(const std::string& loggerName = "root");
    void registerLogger(const std::string& loggerName, const std::shared_ptr<Logger>& logger);
    void removeLogger(const std::string& loggerName);

private:
    std::unordered_map<std::string, std::shared_ptr<Logger>> m_loggerMap;
    std::mutex m_mapMutex;
};
}  // namespace logging
#endif  // LOGGER_FACTORY_H
