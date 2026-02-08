#ifndef LOGGING_FORMATTERS_PATTERN_FORMATTER_H
#define LOGGING_FORMATTERS_PATTERN_FORMATTER_H

#include "logging/details/common.h"
#include "logging/details/log_msg.h"
#include "logging/formatters/formatter.h"

namespace logging {

class PatternFormatter : public Formatter {
public:
    PatternFormatter();
    ~PatternFormatter() override = default;

    explicit PatternFormatter(std::string_view pattern);

    PatternFormatter(std::string_view pattern, std::string_view timePattern);

    void format(const details::LogMsg& logMsg, std::string& logContent) override;
    [[nodiscard]] std::unique_ptr<Formatter> clone() const override;

private:
    void log_msg_to_content(char symbol, const details::LogMsg& logMsg, std::string& logContent);

private:
    std::string _pattern;
    std::string _timePattern;
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_PATTERN_FORMATTER_H
