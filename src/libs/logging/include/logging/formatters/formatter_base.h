#ifndef LOGGING_FORMATTERS_FORMATTER_BASE_H
#define LOGGING_FORMATTERS_FORMATTER_BASE_H

#include <memory>

#include "logging/formatters/formatter.h"
#include "logging/log_msg.h"

namespace logging {
class FormatterBase : public Formatter {
public:
    FormatterBase() = delete;
    ~FormatterBase() override;
    explicit FormatterBase(std::unique_ptr<Formatter> pimpl);

    void format(const LogMsg& logMsg, std::string& content) override;

    [[nodiscard]] std::unique_ptr<Formatter> clone() const override;

protected:
    void throw_if_pimpl_null() const;

private:
    std::unique_ptr<Formatter> _pImpl;
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_FORMATTER_BASE_H
