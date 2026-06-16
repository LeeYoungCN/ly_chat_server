#include "logging/sinks/sink.h"

#include <atomic>
#include <mutex>

#include "common/debug/debug_logger.h"
#include "logging/formatters/pattern_formatter.h"

namespace logging {

struct Sink::Impl {
    std::atomic<LogLevel> level{LogLevel::INFO};
    std::unique_ptr<Formatter> formatter{std::make_unique<PatternFormatter>()};
    std::mutex sinkMtx;
    std::string paramStr;
};

Sink::Sink() : _pimpl(std::make_unique<Impl>()) {}

Sink::~Sink()
{
    if (_pimpl != nullptr) {
        DEBUG_LOGGER_DBG("Sink release. {}.", _pimpl->paramStr);
        _pimpl.reset();
    }
}

bool Sink::should_log(LogLevel level) const
{
    if (level == LogLevel::OFF) {
        return false;
    }
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

void Sink::set_parameter(std::string_view paramStr)
{
    if (_pimpl != nullptr) {
        _pimpl->paramStr = paramStr;
    }
}
}  // namespace logging
