#ifndef LOGGING_FORMATTERS_PATTERN_FORMATTER_H
#define LOGGING_FORMATTERS_PATTERN_FORMATTER_H

#include <memory>

#include "common/compiler/macros.h"
#include "logging/details/log_msg.h"
#include "logging/formatters/formatter.h"

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
class COMMON_API PatternFormatter : public Formatter {
public:
    PatternFormatter();
    ~PatternFormatter() override;

    explicit PatternFormatter(std::string_view pattern);

    PatternFormatter(std::string_view pattern, std::string_view timePattern);

    void format(const details::LogMsg& logMsg, std::string& logContent) override;
    [[nodiscard]] std::unique_ptr<Formatter> clone() const override;

private:
    void log_msg_to_content(char symbol, const details::LogMsg& logMsg, std::string& logContent);

private:
    struct Impl;
    std::unique_ptr<Impl> _pimpl;
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_PATTERN_FORMATTER_H
