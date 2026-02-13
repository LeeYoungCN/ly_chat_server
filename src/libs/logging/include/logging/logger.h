#ifndef LOGGING_LOGGER_H
#define LOGGING_LOGGER_H

#include <atomic>
#include <format>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "common/compiler/macros.h"
#include "common/types/type_traits.h"
#include "common/utils/string_utils.h"
#include "logging/details//inner_macros.h"
#include "logging/details/common.h"
#include "logging/details/log_msg.h"
#include "logging/details/log_source.h"
#include "logging/log_level.h"
#include "logging/sinks/sink.h"

namespace logging {
class COMMON_API Logger {
public:
    Logger() = default;
    virtual ~Logger();

    explicit Logger(std::string_view name);

    Logger(std::string_view name, const std::shared_ptr<Sink>& sink);

    Logger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks);

    Logger(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks);

    template <typename It>
    Logger(std::string_view name, It begin, It end) : _name(name), _sinks(begin, end)
    {
    }

    std::string_view name() const;
    const std::vector<std::shared_ptr<Sink>>& sinks() const;

    void set_level(LogLevel level);
    LogLevel level() const;
    bool should_log(LogLevel level) const;

    void flush_on(LogLevel level);
    LogLevel flush_level() const;
    bool should_flush(LogLevel level) const;

    void set_pattern(std::string_view pattern = FORMATTER_DEFAULT_PATTERN,
                     std::string_view timePattern = FORMATTER_DEFAULT_TIME_PATTERN);
    void sef_formatter(std::unique_ptr<Formatter> formatter);

    void flush();

#pragma region log function

    template <class T,
              std::enable_if_t<common::type_traits::is_convertible_to_string_v<T>, int> = 0>
    void log(logging::details::LogSource source, LogLevel level, const T& message)
    {
        if (!should_log(level)) {
            return;
        }
        details::LogMsg logMsg(source, _name, level, common::string::type_to_string(message));
        sink_it(logMsg);
    }

    template <typename... Args>
    void log(logging::details::LogSource source, LogLevel level, std::format_string<Args...> format,
             Args&&... args)
    {
        if (!should_log(level)) {
            return;
        }
        details::LogMsg logMsg(
            source, _name, level, std::format(format, std::forward<Args>(args)...));
        sink_it(logMsg);
    }

    DEFINE_LOGGER_LOG_FUNCTION(debug, logging::LogLevel::DEBUG)

    DEFINE_LOGGER_LOG_FUNCTION(info, logging::LogLevel::INFO)

    DEFINE_LOGGER_LOG_FUNCTION(warn, logging::LogLevel::WARN)

    DEFINE_LOGGER_LOG_FUNCTION(error, logging::LogLevel::ERROR)

    DEFINE_LOGGER_LOG_FUNCTION(fatal, logging::LogLevel::FATAL)
#pragma endregion

protected:
    virtual void sink_it(const details::LogMsg& logMsg);
    virtual void flush_it();

    void sinks_log_it(const details::LogMsg& logMsg);
    void sinks_flush_it();

private:
    std::string _name;
    std::vector<std::shared_ptr<Sink>> _sinks;
    std::atomic<LogLevel> _level{LogLevel::INFO};
    std::atomic<LogLevel> _flushLevel{LogLevel::OFF};
};
}  // namespace logging

#endif  // LOGGING_LOGGER_H
