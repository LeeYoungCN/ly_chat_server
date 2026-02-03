#include "logging/logger.h"

#include <cstdarg>
#include <utility>

#include "logging/details/log_level.h"

namespace logging {
using namespace details;

Logger::Logger(std::string name) : _name(std::move(name)) {}

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
    return (_logLevel != LOG_LVL_OFF && level >= _logLevel);
}

void Logger::log(LogSource source, LogLevel level, std::string message)
{
    if (!should_log(level)) {
        return;
    }

    log_it(LogMsg(this->_name, level, std::move(message), source));
}

void Logger::log_it(LogMsg&& logMsg)
{
    for (const auto& sink : _sinkList) {
        sink->log(logMsg);
    }

    if (_flushLevel != LogLevel::LOG_LVL_OFF && logMsg.level >= _flushLevel) {
        flush();
    }
}

void Logger::flush()
{
    flush_it();
}

void Logger::flush_it()
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
