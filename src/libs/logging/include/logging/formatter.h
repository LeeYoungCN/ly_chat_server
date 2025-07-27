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

#include "logging/logging_types.h"

namespace logging {
class Formatter {
public:
    Formatter() = default;
    ~Formatter() = default;
    Formatter(const std::string& name, const std::string& pattern);
    void setName(const std::string& name);
    void getName();

private:
    std::string m_name;
    std::string m_pattern;
};
}  // namespace logging
#endif  // LOGGING_FORMATTER_H
