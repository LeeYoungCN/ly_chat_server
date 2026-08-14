#ifndef LOGGING_LOG_SOURCE_H
#define LOGGING_LOG_SOURCE_H

#include <string>

namespace logging {
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
}  // namespace logging

#define LOG_SRC_LOCAL                  logging::LogSource(__FILE__, __LINE__, __FUNCTION__)
#define LOG_SRC_EMPTY                  logging::LogSource()
#define LOG_SRC_INST(file, line, func) logging::LogSource(file, line, func)

#endif  // LOGGING_LOG_SOURCE_H
