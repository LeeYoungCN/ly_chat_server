#ifndef LOGGING_FORMATTERS_FORMATTER_H
#define LOGGING_FORMATTERS_FORMATTER_H

#include "logging/details/log_msg.h"

namespace logging {
class Formatter {
public:
    Formatter() = default;
    virtual ~Formatter() = default;

    virtual void format(const details::LogMsg& logMsg, std::string& content) = 0;
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_FORMATTER_H
