#include "logging/logger.h"

#include <cstdarg>
#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/log_level.h"

namespace logging {
using namespace details;

Logger::~Logger()
{
    DEBUG_LOGGER_INFO("Logger release. [{}]", name());
}

Logger::Logger(std::string_view name) : _name(name) {}

Logger::Logger(std::string_view name, const std::shared_ptr<Sink>& sink) : Logger(name, {sink}) {}

Logger::Logger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks)
    : Logger(name, sinks.begin(), sinks.end())
{
}

Logger::Logger(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks)
    : Logger(name, sinks.begin(), sinks.end())
{
}

void Logger::set_level(LogLevel level)
{
    _level = level;
}

LogLevel Logger::level() const
{
    return _level;
}

bool Logger::should_log(LogLevel level) const
{
    return (level != LogLevel::OFF && level >= _level);
}

void Logger::flush_on(LogLevel level)
{
    _flushLevel = level;
}

LogLevel Logger::flush_level() const
{
    return _flushLevel;
}

bool Logger::should_flush(LogLevel level) const
{
    return (level != LogLevel::OFF && level >= _flushLevel);
}

std::string_view Logger::name() const
{
    return this->_name;
}

void Logger::set_pattern(std::string_view pattern, std::string_view timePattern)
{
    for (const auto& sink : _sinks) {
        sink->set_pattern(pattern, timePattern);
    }
}

void Logger::sef_formatter(std::unique_ptr<Formatter> formatter)
{
    for (uint32_t i = 0; i < _sinks.size(); i++) {
        if (i == _sinks.size() - 1) {
            _sinks[i]->set_formatter(std::move(formatter));
        } else {
            _sinks[i]->set_formatter(formatter->clone());
        }
    }
}

const std::vector<std::shared_ptr<Sink>>& Logger::sinks() const
{
    return _sinks;
}

void Logger::flush()
{
    flush_it();
}

void Logger::log(details::LogSource source, LogLevel level, const char* format, va_list args)
{
    if (!should_log(level)) {
        return;
    }
    details::LogMsg logMsg(source, _name, level, common::string::va_list_to_string(format, args));
    sink_it(logMsg);
}

void Logger::sink_it(const details::LogMsg& logMsg)
{
    sinks_log_it(logMsg);

    if (should_flush(logMsg.level)) {
        sinks_flush_it();
    }
}

void Logger::flush_it()
{
    sinks_flush_it();
}

void Logger::sinks_log_it(const LogMsg& logMsg)
{
    for (const auto& sink : _sinks) {
        if (sink->should_log(logMsg.level)) {
            sink->log(logMsg);
        }
    }
}

void Logger::sinks_flush_it()
{
    for (const auto& sink : _sinks) {
        sink->flush();
    }
}

}  // namespace logging
