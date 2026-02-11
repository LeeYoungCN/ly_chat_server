#include "logging/sinks/sink.h"

#include <atomic>

namespace logging {

bool Sink::should_log(LogLevel level) const
{
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

void Sink::set_pattern(std::string_view pattern, std::string_view timePattern)
{
    set_formatter(std::make_unique<PatternFormatter>(pattern, timePattern));
}

void Sink::set_formatter(std::unique_ptr<Formatter> formatter)
{
    std::lock_guard lock(_sinkMtx);
    _formatter = std::move(formatter);
}
}  // namespace logging
