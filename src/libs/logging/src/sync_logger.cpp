#include "logging/sync_logger.h"

#include <atomic>
#include <memory>
#include <utility>
#include <vector>

#include "common/debug/debug_logger.h"
#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"

namespace logging {
using namespace details;

struct SyncLogger::Impl {
    std::string name;
    std::vector<std::shared_ptr<Sink>> sinks;
    std::atomic<LogLevel> level{LogLevel::INFO};
    std::atomic<LogLevel> flushLevel{LogLevel::OFF};

    explicit Impl(std::string_view name) : name(name) {}

    Impl(std::string_view name, std::vector<std::shared_ptr<Sink>> sinks)
        : name(name), sinks(std::move(sinks))
    {
    }
};

SyncLogger::~SyncLogger()
{
    DEBUG_LOGGER_DBG("Logger release. Name: \"{}\", SinkCount: {}.", name(), _pimpl->sinks.size());
    _pimpl.reset();
}

SyncLogger::SyncLogger(std::string_view name) : _pimpl(std::make_unique<Impl>(name)) {}

SyncLogger::SyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink)
    : _pimpl(std::make_unique<Impl>(name, std::vector<std::shared_ptr<Sink>>{sink}))
{
}

SyncLogger::SyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks)
    : _pimpl(std::make_unique<Impl>(name, sinks))
{
}

SyncLogger::SyncLogger(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks)
    : _pimpl(std::make_unique<Impl>(name, std::vector<std::shared_ptr<Sink>>(sinks)))
{
}

std::string_view SyncLogger::name() const
{
    return _pimpl->name;
}

const std::vector<std::shared_ptr<Sink>>& SyncLogger::sinks() const
{
    return _pimpl->sinks;
}

void SyncLogger::set_level(LogLevel level) const
{
    _pimpl->level.store(level, std::memory_order_relaxed);
}

LogLevel SyncLogger::level() const
{
    return _pimpl->level.load(std::memory_order_relaxed);
}

bool SyncLogger::should_log(LogLevel level) const
{
    return (level != LogLevel::OFF && level >= this->level());
}

void SyncLogger::flush_on(LogLevel level) const
{
    _pimpl->flushLevel.store(level, std::memory_order_relaxed);
}

LogLevel SyncLogger::flush_level() const
{
    return _pimpl->flushLevel.load(std::memory_order_relaxed);
}

bool SyncLogger::should_flush(LogLevel level) const
{
    return (level != LogLevel::OFF && level >= this->flush_level());
}

void SyncLogger::set_pattern(std::string_view pattern) const
{
    set_formatter(std::make_unique<PatternFormatter>(pattern));
}

void SyncLogger::set_formatter(const std::unique_ptr<Formatter>& formatter) const
{
    for (auto& sink : _pimpl->sinks) {
        sink->set_formatter(formatter->clone());
    }
}

void SyncLogger::flush()
{
    flush_it();
}

void SyncLogger::log(const details::LogMsg& logMsg)
{
    log_it(logMsg);
}

void SyncLogger::log_it(const details::LogMsg& logMsg)
{
    sinks_log_it(logMsg);

    if (should_flush(logMsg.level)) {
        sinks_flush_it();
    }
}

void SyncLogger::flush_it()
{
    sinks_flush_it();
}

void SyncLogger::sinks_log_it(const LogMsg& logMsg)
{
    for (const auto& sink : _pimpl->sinks) {
        if (sink->should_log(logMsg.level)) {
            sink->log(logMsg);
        }
    }
}

void SyncLogger::sinks_flush_it()
{
    for (const auto& sink : _pimpl->sinks) {
        sink->flush();
    }
}

}  // namespace logging
