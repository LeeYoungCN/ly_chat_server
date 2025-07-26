/**
 * @file appder_fortory.h
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

#ifndef APPENDER_FACTORY_H
#define APPENDER_FACTORY_H

#include "common/base/singleton.h"
#include "logging.h"

namespace logging {
class AppenderFactory : public common::base::SingletonBase<AppenderFactory> {
public:
    std::shared_ptr<Appender> getAppender(const std::string& appenderName, AppenderType type);
    void registerAppender(const std::string& appenderName, const std::shared_ptr<Appender>& appender);
    void removeAppender(const std::string& appenderName);

private:
    std::unordered_map<std::string, std::shared_ptr<Appender>> m_appenderMap;
    std::mutex m_mapMutex;
};
}  // namespace logging
#endif  // APPENDER_FACTORY_H
