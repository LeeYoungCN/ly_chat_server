/**
 * @file logging_base.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef LOGGING_DETAILS_LOGGING_BASE_H
#define LOGGING_DETAILS_LOGGING_BASE_H

#include <string>

namespace logging::details {
class LoggingBase {
public:
    LoggingBase() = default;
    virtual ~LoggingBase() = default;
    explicit LoggingBase(std::string name) : m_name(std::move(name)) {};
    [[nodiscard]] const std::string& getName() const { return m_name; };
    void setName(std::string name) { m_name = std::move(name); };

protected:
    std::string m_name;
};
}  // namespace logging::details

#endif  // LOGGING_DETAILS_LOGGING_BASE_H
