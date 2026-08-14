#include "loggers/internal/logger_impl.h"

#include <atomic>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <vector>

#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"
#include "logging/log_msg.h"
#include "logging/log_source.h"
#include "logging/sinks/sink.h"

namespace logging {

LoggerImpl::LoggerImpl(std::string_view name) : _name(name)
{
    if (name.empty()) {
        throw std::invalid_argument("Logger name cannot be empty.");
    }
}

LoggerImpl::LoggerImpl(std::string_view name, const std::shared_ptr<logging::Sink>& sink)
    : _name(name), _sinks{sink}
{
    if (name.empty()) {
        throw std::invalid_argument("Logger name cannot be empty.");
    }

    if (sink == nullptr) {
        throw std::invalid_argument("Sink cannot be null.");
    }
}

LoggerImpl::LoggerImpl(std::string_view name,
                       const std::vector<std::shared_ptr<logging::Sink>>& sinks)
    : _name(name), _sinks(sinks)
{
    if (name.empty()) {
        throw std::invalid_argument("Logger name cannot be empty.");
    }

    for (const auto& sink : sinks) {
        if (sink == nullptr) {
            throw std::invalid_argument("Sink cannot be null.");
        }
    }
}

LoggerImpl::LoggerImpl(std::string_view name,
                       const std::initializer_list<std::shared_ptr<logging::Sink>>& sinks)
    : _name(name), _sinks(sinks)
{
    if (name.empty()) {
        throw std::invalid_argument("Logger name cannot be empty.");
    }

    for (const auto& sink : sinks) {
        if (sink == nullptr) {
            throw std::invalid_argument("Sink cannot be null.");
        }
    }
}

std::string_view LoggerImpl::name() const
{
    return _name;
}

const std::vector<std::shared_ptr<Sink>>& LoggerImpl::sinks() const
{
    return _sinks;
}

void LoggerImpl::set_level(LogLevel level)
{
    _level.store(level, std::memory_order_relaxed);
}

LogLevel LoggerImpl::level() const
{
    return _level.load(std::memory_order_relaxed);
}

bool LoggerImpl::should_log(LogLevel level) const
{
    return (level != LogLevel::OFF && level >= this->level());
}

void LoggerImpl::flush_on(LogLevel level)
{
    _flushLevel.store(level, std::memory_order_relaxed);
}

LogLevel LoggerImpl::flush_level() const
{
    return _flushLevel.load(std::memory_order_relaxed);
}

bool LoggerImpl::should_flush(LogLevel level) const
{
    return (level != LogLevel::OFF && level >= this->flush_level());
}

void LoggerImpl::set_pattern(std::string_view pattern) const
{
    set_formatter(std::make_unique<PatternFormatter>(pattern));
}

void LoggerImpl::set_formatter(const std::unique_ptr<Formatter>& formatter) const
{
    for (auto& sink : _sinks) {
        sink->set_formatter(formatter->clone());
    }
}

void LoggerImpl::log(const LogSource& source, LogLevel level, std::string_view message)
{
    log_it(LogMsg(source, name(), level, message));
}

void LoggerImpl::flush()
{
    flush_it();
}

void LoggerImpl::backend_log(const LogMsg& logMsg)
{
    for (const auto& sink : _sinks) {
        if (sink->should_log(logMsg.level)) {
            sink->log(logMsg);
        }
    }
}

void LoggerImpl::backend_flush()
{
    for (const auto& sink : _sinks) {
        sink->flush();
    }
}

}  // namespace logging
