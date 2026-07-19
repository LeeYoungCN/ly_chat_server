#include "internal/sinks/base_sink_impl.h"

#include <atomic>
#include <mutex>
#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/formatters/pattern_formatter.h"

namespace logging {

BaseSinkImpl::~BaseSinkImpl()
{
    DEBUG_LOGGER_DBG("Release Sink. {}", _paramStr);
}

BaseSinkImpl::BaseSinkImpl(std::string_view parameter) : _paramStr(parameter)
{
    DEBUG_LOGGER_DBG("Create Sink. {}", _paramStr);
}

void BaseSinkImpl::log(const details::LogMsg& logMsg)
{
    std::lock_guard lock(_sinkMtx);
    log_it(logMsg);
}

void BaseSinkImpl::flush()
{
    std::lock_guard lock(_sinkMtx);
    flush_it();
}

bool BaseSinkImpl::should_log(LogLevel level) const
{
    if (level == LogLevel::OFF) {
        return false;
    }
    return level >= _level.load(std::memory_order_relaxed);
}

void BaseSinkImpl::set_level(LogLevel level)
{
    _level.store(level, std::memory_order_relaxed);
};

LogLevel BaseSinkImpl::level() const
{
    return _level.load(std::memory_order_relaxed);
}

void BaseSinkImpl::set_pattern(std::string_view pattern)
{
    set_formatter(std::make_unique<PatternFormatter>(pattern));
}

void BaseSinkImpl::set_formatter(std::unique_ptr<Formatter> formatter)
{
    std::lock_guard lock(_sinkMtx);
    _formatter = std::move(formatter);
}

}  // namespace logging
