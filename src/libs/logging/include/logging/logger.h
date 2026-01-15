#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <atomic>
#include <string_view>
#include <vector>

#include "common/types/logging_types.h"
#include "logging/log_sink.h"

namespace logging {
class Logger {
public:
    Logger() = default;
    explicit Logger(std::string_view name);
    ~Logger() = default;

    void setLevel(LogLevel level);
    void setName(std::string_view name);

    void addSink(LogSink *sink);
    void dispose();

    void log(LogLevel level, const char *format, ...);
    void log(const char *file, int line, const char *func, LogLevel level, const char *format, ...);
    void debug();
    void info();
    void warn();
    void error();
    void fatal();

private:
    void flush();
    bool shouldLog();

private:
    std::atomic<LogLevel> m_level = LogLevel::LOG_LVL_INFO;
    std::vector<LogSink *> m_sinkList;
    std::string m_name{"Default"};
};
}  // namespace logging

#endif  // LOGGING_LOGGER_H
