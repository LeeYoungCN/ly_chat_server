#ifndef LOGGING_LOG_SOURCE_H
#define LOGGING_LOG_SOURCE_H

#include <string_view>

#define LOG_SRC_ST logging::LogSource(__FILE__, __LINE__, __FUNCTION__)
#define LOG_SRC __FILE__, __LINE__, __FUNCTION__
namespace logging {
    constexpr int INVALID_LINE_NUM = -1;

    struct LogSource {
    std::string_view file;
    int line{INVALID_LINE_NUM};
    std::string_view func;

    LogSource() = default;
    LogSource(std::string_view filePath, int lineNumber, std::string_view funcName)
        : file(filePath), line(lineNumber), func(funcName) { }

    [[nodiscard]] bool IsValid() const
    {
        return (!file.empty() && line != INVALID_LINE_NUM);
    }
};
}  // namespace logging

#endif  // LOGGING_LOG_SOURCE_H
