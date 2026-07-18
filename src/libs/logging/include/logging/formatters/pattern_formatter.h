#ifndef LOGGING_FORMATTERS_PATTERN_FORMATTER_H
#define LOGGING_FORMATTERS_PATTERN_FORMATTER_H

#include <memory>

#include "logging/details/constants.h"
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
class PatternFormatter : public Formatter {
public:
    PatternFormatter() = default;
    ~PatternFormatter()  override = default;

    explicit PatternFormatter(std::string_view pattern);

    void format(const details::LogMsg& logMsg, std::string& logContent) override;
    [[nodiscard]] std::unique_ptr<Formatter> clone() const override;

private:
    void log_msg_to_content(char symbol, const details::LogMsg& logMsg, std::string& logContent);
    void format_time(const details::LogMsg& logMsg, std::string& logContent);

private:
    const std::string _pattern{details::FORMATTER_DEFAULT_PATTERN};
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_PATTERN_FORMATTER_H
