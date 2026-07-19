#ifndef LOGGING_FORMATTERS_FORMATTER_H
#define LOGGING_FORMATTERS_FORMATTER_H

#include <memory>

#include "logging/details/log_msg.h"

namespace logging {
class FormatterImpl;

class Formatter {
public:
    Formatter() = delete;
    virtual ~Formatter();
    explicit Formatter(std::unique_ptr<FormatterImpl> pimpl);

    void format(const details::LogMsg& logMsg, std::string& content);

    [[nodiscard]] std::unique_ptr<Formatter> clone() const;

private:
    std::unique_ptr<FormatterImpl> _pImpl;
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_FORMATTER_H
