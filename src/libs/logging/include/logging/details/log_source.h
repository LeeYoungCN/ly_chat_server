#ifndef LOGGING_DETAILS_LOG_SOURCE_H
#define LOGGING_DETAILS_LOG_SOURCE_H

#include <string_view>

#define LOG_SRC_ST logging::details::LogSource(__FILE__, __LINE__, __FUNCTION__)

namespace logging::details {
constexpr int INVALID_LINE_NUM = -1;

struct LogSource {
    std::string_view file;
    int line{INVALID_LINE_NUM};
    std::string_view func;

    LogSource() = default;
    LogSource(std::string_view filePath, int lineNumber, std::string_view funcName);

    [[nodiscard]] bool IsValid() const;
};
}  // namespace logging::details

#endif  // LOGGING_DETAILS_LOG_SOURCE_H
