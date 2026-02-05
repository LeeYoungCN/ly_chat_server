#include "logging/logger.h"

#include <cstdarg>
#include <utility>

#include "logging/log_level.h"

namespace logging {
using namespace details;

Logger::Logger(std::string name) : _name(std::move(name)) {}

void Logger::log(details::LogSource source, LogLevel level, const char* format, va_list args)
{
    if (!should_log(level)) {
        return;
    }
    details::LogMsg logMsg(source, _name, level, common::string::va_list_to_string(format, args));
    log_it(logMsg);
}

void Logger::set_log_level(LogLevel level)
{
    _logLevel = level;
}

void Logger::set_flush_level(LogLevel level)
{
    _autoFlush = true;
    _flushLevel = level;
}

void Logger::set_name(std::string name)
{
    this->_name = std::move(name);
}

std::string_view Logger::get_name() const
{
    return this->_name;
}

void Logger::add_sink(const std::shared_ptr<Sink>& sink)
{
    _sinkList.emplace_back(sink);
}

bool Logger::should_log(LogLevel level) const
{
    return (level >= _logLevel);
}

void Logger::log_it(const details::LogMsg& logMsg)
{
    sinks_log_it(logMsg);

    if (logMsg.level >= _flushLevel) {
        flush();
    }
}

void Logger::flush()
{
    flush_it();
}

void Logger::flush_it()
{
    sinks_flush_it();
}

void Logger::sinks_log_it(const LogMsg& logMsg)
{
    for (const auto& sink : _sinkList) {
        if (sink->should_log(logMsg.level)) {
            sink->log(logMsg);
        }
    }
}

void Logger::sinks_flush_it()
{
    for (const auto& sink : _sinkList) {
        sink->flush();
    }
}

Logger::~Logger()
{
    flush();
    _sinkList.clear();
};

}  // namespace logging
