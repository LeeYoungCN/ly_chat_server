/**
 * @file consol_appender.h
 * @author your name (you@domain.com)
 * @brief 输出器, 输出到控制台。单例设计，全局唯一。
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef CONSOLE_APPENDER_H
#define CONSOLE_APPENDER_H

#include "common/base/singleton.h"
#include "logging/appender.h"

namespace logging {
class ConsoleAppender : public Appender, public common::base::SingletonBase<ConsoleAppender> {
public:
    ConsoleAppender() { setName("ConsoleAppender"); };
    ~ConsoleAppender() override = default;
    void flush(const std::string& message) override;
};
}  // namespace logging

#endif  // CONSOLE_APPENDER_H
