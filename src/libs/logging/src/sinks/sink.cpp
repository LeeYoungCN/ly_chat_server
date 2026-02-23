#include "logging/sinks/sink.h"

#include <atomic>
#include <mutex>

#include "logging/formatters/pattern_formatter.h"

namespace logging {

struct Sink::Impl {
    std::atomic<LogLevel> level{LogLevel::INFO};
    std::unique_ptr<Formatter> formatter{std::make_unique<PatternFormatter>()};
    std::mutex sinkMtx;
};

Sink::Sink() : _pimpl(new Impl()) {}

Sink::~Sink()
{
    delete _pimpl;
}

bool Sink::should_log(LogLevel level) const
{
    return level >= _pimpl->level.load(std::memory_order_relaxed);
}

void Sink::set_level(LogLevel level)
{
    _pimpl->level.store(level, std::memory_order_relaxed);
};

LogLevel Sink::level() const
{
    return _pimpl->level.load(std::memory_order_relaxed);
}

void Sink::set_pattern(std::string_view pattern, std::string_view timePattern)
{
    set_formatter(std::make_unique<PatternFormatter>(pattern, timePattern));
}

void Sink::set_formatter(std::unique_ptr<Formatter> formatter)
{
    std::lock_guard lock(_pimpl->sinkMtx);
    _pimpl->formatter = std::move(formatter);
}

std::mutex& Sink::sink_mutex()
{
    return _pimpl->sinkMtx;
}

std::unique_ptr<Formatter>& Sink::formatter()
{
    return _pimpl->formatter;
}
}  // namespace logging
