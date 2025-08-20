/**
 * @file appender_base.h
 * @author LiYang
 * @brief
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef LOGGING_APPENDER_H
#define LOGGING_APPENDER_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string>

#include "common/types/logging_types.h"
#include "logging/details/log_record.h"
#include "logging/details/logging_base.h"
#include "logging/formatter.h"

namespace logging {
enum AppenderType { CONSOLE_APPENDER, FILE, ROLLING_FILE };

class Appender : public details::LoggingBase {
public:
    Appender() = default;
    ~Appender() override = default;
    explicit Appender(std::string name) : logging::details::LoggingBase(std::move(name)) {};

    void setFormater(std::unique_ptr<logging::Formatter> formater)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_formatter = std::move(formater);
    };

    virtual void append(logging::details::LogRecord record);
    void setLevel(common::types::logging::LogLevel level) { return m_level.store(level); };
    bool shouldLog(common::types::logging::LogLevel level) const
    {
        return m_level.load(std::memory_order_relaxed) <= level;
    };

protected:
    virtual void flush(const std::string& message) = 0;

protected:
    std::mutex m_mutex;
    std::atomic<common::types::logging::LogLevel> m_level{common::types::logging::LogLevel::INFO};
    std::unique_ptr<logging::Formatter> m_formatter{std::make_unique<Formatter>("default")};
};
}  // namespace logging
#endif  // LOGGING_APPENDER_H
