/**
 * @file base_formatter.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-24
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef LOGGING_FORMATTER_H
#define LOGGING_FORMATTER_H
#include <string>

#include "logging/details/log_record.h"
#include "logging/details/logging_base.h"

namespace logging {
class Formatter : public details::LoggingBase {
public:
    Formatter() = default;
    ~Formatter() override = default;
    explicit Formatter(std::string name) : details::LoggingBase(std::move(name)) {};
    Formatter(const std::string& name, const std::string& pattern);
    virtual std::string format(const details::LogRecord& record);
};
}  // namespace logging
#endif  // LOGGING_FORMATTER_H
