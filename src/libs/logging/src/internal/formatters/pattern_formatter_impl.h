#ifndef LOGGING_INTERNAL_FORMATTERS_PATTERN_FORMATTER_IMPL_H
#define LOGGING_INTERNAL_FORMATTERS_PATTERN_FORMATTER_IMPL_H

#include <memory>

#include "internal/formatters/formatter_impl.h"
#include "logging/details/constants.h"
#include "logging/details/log_msg.h"

namespace logging {
/**
 * @brief Format log message by pattern.
 *
 * d: Date time;
 * n: Logger name;
 * l: Log level short name;
 * L: Log level full name;
 * s: File name;
 * g: File path;
 * #: Line number;
 * !: Function name;
 * t: Thread ID;
 * P: Process ID;
 * v: Log message;
 */
class PatternFormatterImpl : public FormatterImpl {
public:
    PatternFormatterImpl() = default;
    ~PatternFormatterImpl() override = default;

    explicit PatternFormatterImpl(std::string_view pattern);

    void format(const details::LogMsg& logMsg, std::string& logContent) override;
    [[nodiscard]] std::unique_ptr<FormatterImpl> clone() const override;

private:
    void log_msg_to_content(char symbol, const details::LogMsg& logMsg, std::string& logContent);
    void format_time(const details::LogMsg& logMsg, std::string& logContent);

private:
    const std::string _pattern{details::FORMATTER_DEFAULT_PATTERN};
};
}  // namespace logging

#endif  // LOGGING_INTERNAL_FORMATTERS_PATTERN_FORMATTER_IMPL_H
