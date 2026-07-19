#ifndef LOGGING_INTERNAL_FORMATTERS_FORMATTER_IMPL_H
#define LOGGING_INTERNAL_FORMATTERS_FORMATTER_IMPL_H

#include <memory>

#include "logging/details/log_msg.h"

namespace logging {
class FormatterImpl {
public:
    FormatterImpl() = default;
    virtual ~FormatterImpl() = default;

    virtual void format(const details::LogMsg& logMsg, std::string& content) = 0;

    [[nodiscard]] virtual std::unique_ptr<FormatterImpl> clone() const = 0;
};
}  // namespace logging

#endif  // LOGGING_INTERNAL_FORMATTERS_FORMATTER_IMPL_H
