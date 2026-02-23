#include "logging/logger.h"

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

struct Logger::Impl {
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

Logger::~Logger()
{
    DEBUG_LOGGER_INFO("Logger release. [{}]", name());
    delete _pimpl;
}

Logger::Logger(std::string_view name) : _pimpl(new Impl(name)) {}

Logger::Logger(std::string_view name, const std::shared_ptr<Sink>& sink)
    : _pimpl(new Impl(name, {sink}))
{
}

Logger::Logger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks)
    : _pimpl(new Impl(name, sinks))
{
}

Logger::Logger(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks)
    : _pimpl(new Impl(name, std::vector<std::shared_ptr<Sink>>(sinks)))
{
}

std::string_view Logger::name() const
{
    return _pimpl->name;
}

const std::vector<std::shared_ptr<Sink>>& Logger::sinks() const
{
    return _pimpl->sinks;
}

void Logger::set_level(LogLevel level)
{
    _pimpl->level.store(level, std::memory_order_relaxed);
}

LogLevel Logger::level() const
{
    return _pimpl->level.load(std::memory_order_relaxed);
}

bool Logger::should_log(LogLevel level) const
{
    return (level != LogLevel::OFF && level >= this->level());
}

void Logger::flush_on(LogLevel level)
{
    _pimpl->flushLevel.store(level, std::memory_order_relaxed);
}

LogLevel Logger::flush_level() const
{
    return _pimpl->flushLevel.load(std::memory_order_relaxed);
}

bool Logger::should_flush(LogLevel level) const
{
    return (level != LogLevel::OFF && level >= this->flush_level());
}

void Logger::set_pattern(std::string_view pattern, std::string_view timePattern)
{
    set_formatter(std::make_unique<PatternFormatter>(pattern, timePattern));
}

void Logger::set_formatter(const std::unique_ptr<Formatter>& formatter)
{
    for (auto& sink : _pimpl->sinks) {
        sink->set_formatter(formatter->clone());
    }
}

void Logger::flush()
{
    flush_it();
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
    for (const auto& sink : _pimpl->sinks) {
        if (sink->should_log(logMsg.level)) {
            sink->log(logMsg);
        }
    }
}

void Logger::sinks_flush_it()
{
    for (const auto& sink : _pimpl->sinks) {
        sink->flush();
    }
}

}  // namespace logging
