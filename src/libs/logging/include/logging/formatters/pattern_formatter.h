#ifndef LOGGING_FORMATTERS_PATTERN_FORMATTER_H
#define LOGGING_FORMATTERS_PATTERN_FORMATTER_H

#include "logging/details/common.h"
#include "logging/details/log_msg.h"
#include "logging/formatters/formatter.h"

namespace logging {

class PatternFormatter : public Formatter {
public:
    PatternFormatter() = default;
    ~PatternFormatter() override = default;

    explicit PatternFormatter(std::string pattern);

    PatternFormatter(std::string pattern, std::string timePattern);

    void format(const details::LogMsg& logMsg, std::string& logContent) override;
    [[nodiscard]] std::unique_ptr<Formatter> clone() const override;

private:
    void log_msg_to_content(char symbol, const details::LogMsg& logMsg, std::string& logContent);

private:
    std::string _pattern = DEFAULT_PATTERN;
    std::string _timePattern = DEFAULT_TIME_PATTERN;
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_PATTERN_FORMATTER_H
