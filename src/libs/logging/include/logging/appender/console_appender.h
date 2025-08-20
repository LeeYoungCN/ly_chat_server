/**
 * @file consol_appender.h
 * @author your name (you@domain.com)
 * @brief 输出器, 输出到控制台。
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef LOGGING_CONSOLE_APPENDER_H
#define LOGGING_CONSOLE_APPENDER_H

#include "logging/appender/appender.h"

namespace logging {
class ConsoleAppender : public Appender {
public:
    ConsoleAppender() = default;
    ~ConsoleAppender() override = default;
    explicit ConsoleAppender(std::string name) : Appender(std::move(name)) {};
    void flush(const std::string& message) override;
};
}  // namespace logging

#endif  // LOGGING_CONSOLE_APPENDER_H
