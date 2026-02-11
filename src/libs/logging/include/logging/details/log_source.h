#ifndef LOGGING_DETAILS_LOG_SOURCE_H
#define LOGGING_DETAILS_LOG_SOURCE_H

#include <string>

namespace logging::details {
constexpr int INVALID_LINE_NUM = -1;

struct LogSource {
    std::string file;
    int line{INVALID_LINE_NUM};
    std::string func;

    LogSource() = default;
    LogSource(std::string_view filePath, int lineNumber, std::string_view funcName)
        : file(filePath), line(lineNumber), func(funcName) {};

    [[nodiscard]] bool empty() const { return (file.empty() || line == INVALID_LINE_NUM); }
};
}  // namespace logging::details

#endif  // LOGGING_DETAILS_LOG_SOURCE_H
