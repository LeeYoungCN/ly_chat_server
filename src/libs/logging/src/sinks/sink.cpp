#include "logging/sinks/sink.h"

#include <atomic>
#include <mutex>

#include "logging/formatters/pattern_formatter.h"

namespace logging {

Sink::Sink(std::string_view parameter) : _paramStr(parameter) {}

void Sink::log(const details::LogMsg& logMsg)
{
    std::lock_guard lock(_sinkMtx);
    log_it(logMsg);
}

void Sink::flush()
{
    std::lock_guard lock(_sinkMtx);
    flush_it();
}

bool Sink::should_log(LogLevel level) const
{
    if (level == LogLevel::OFF) {
        return false;
    }
    return level >= _level.load(std::memory_order_relaxed);
}

void Sink::set_level(LogLevel level)
{
    _level.store(level, std::memory_order_relaxed);
};

LogLevel Sink::level() const
{
    return _level.load(std::memory_order_relaxed);
}

void Sink::set_pattern(std::string_view pattern)
{
    set_formatter(std::make_unique<PatternFormatter>(pattern));
}

void Sink::set_formatter(std::unique_ptr<Formatter> formatter)
{
    std::lock_guard lock(_sinkMtx);
    _formatter = std::move(formatter);
}

}  // namespace logging
