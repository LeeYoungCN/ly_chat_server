/**
 * @file appender_base.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef LOGGING_APPENDER_H
#define LOGGING_APPENDER_H

#include <mutex>
#include <string>

#include "common/types/log_level.h"
#include "logging/logging_types.h"

namespace logging {
enum AppenderType { CONSOLE_APPENDER, FILE, ROLLING_FLIE };

class Appender {
public:
    Appender() = default;
    virtual ~Appender() = default;
    void append(const logging::LogRecord& record);
    void setName(const std::string& name);
    std::string getName();

protected:
    virtual void flush(const std::string& message) = 0;

protected:
    std::mutex m_appendMtx;
    std::string m_name = "Appender";
    common::types::LogLevel m_filterLevel = common::types::LogLevel::INFO;
    /**
     * @brief 默认log序列化。
     *
     * @param record 日志信息
     * @return std::string
     */
    std::string defaultFormatLog(const logging::LogRecord& record);
};
}  // namespace logging
#endif  // LOGGING_APPENDER_H
