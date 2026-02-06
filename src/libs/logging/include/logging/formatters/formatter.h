#ifndef LOGGING_FORMATTERS_FORMATTER_H
#define LOGGING_FORMATTERS_FORMATTER_H

#include <memory>

#include "logging/details/log_msg.h"

namespace logging {
class Formatter {
public:
    Formatter() = default;
    virtual ~Formatter() = default;

    virtual void format(const details::LogMsg& logMsg, std::string& content) = 0;

    [[nodiscard]] virtual std::unique_ptr<Formatter> clone() const = 0;
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_FORMATTER_H
