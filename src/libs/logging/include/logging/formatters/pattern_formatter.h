#ifndef LOGGING_FORMATTERS_PATTERN_FORMATTER_H
#define LOGGING_FORMATTERS_PATTERN_FORMATTER_H

#include "logging/details/log_msg.h"
#include "logging/formatters/formatter.h"

namespace logging {
class PatternFormatter : public Formatter {
public:
    PatternFormatter() = default;
    ~PatternFormatter() override = default;

    explicit PatternFormatter(std::string pattern);

    void format(const details::LogMsg& logMsg, std::string& logContent) override;

private:
    void log_msg_to_content(char symbol, const details::LogMsg& logMsg, std::string& logContent);

private:
    std::string _pattern = "[%d][%L][%s:%#]: %v";
};
}  // namespace logging

#endif  // LOGGING_FORMATTERS_PATTERN_FORMATTER_H
