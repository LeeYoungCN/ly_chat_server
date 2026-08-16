#ifndef LOGGING_FORMATTERS_FORMATTER_H
#define LOGGING_FORMATTERS_FORMATTER_H

#include <memory>

#include "logging/log_msg.h"

namespace origin::logging {

class Formatter {
public:
    Formatter() = default;
    virtual ~Formatter() = default;

    virtual void format(const LogMsg& logMsg, std::string& content) = 0;

    [[nodiscard]] virtual std::unique_ptr<Formatter> clone() const = 0;
};
}  // namespace origin::logging

#endif  // LOGGING_FORMATTERS_FORMATTER_H
