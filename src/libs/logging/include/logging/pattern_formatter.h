/**
 * @file pattern_formatter.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef LOGGING_PATTERN_FORMATTER_H
#define LOGGING_PATTERN_FORMATTER_H

#include <string>
#include <utility>

#include "logging/details/log_record.h"
#include "logging/formatter.h"

namespace logging {

class PatternFormatter : public Formatter {
public:
    PatternFormatter() = default;
    ~PatternFormatter() override = default;
    explicit PatternFormatter(std::string name) : Formatter(std::move(name)) {}
    void setPattern(std::string pattern) { m_pattern = std::move(pattern); };
    std::string format(const logging::details::LogRecord& record) override;

private:
    std::string m_pattern;
};
};  // namespace logging
#endif  // LOGGING_PATTERN_FORMATTER_H
