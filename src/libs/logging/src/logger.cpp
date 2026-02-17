#include "logging/logger.h"

#include <atomic>
#include <cstdarg>
#include <memory>

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

    template <typename It>
    Impl(std::string_view name, It begin, It end) : name(name), sinks(begin, end)
    {
    }
};

template <typename It>
Logger::Logger(std::string_view name, It begin, It end)
    : _pimpl(std::make_unique<Impl>(name, begin, end))
{
}

Logger::~Logger()
{
    DEBUG_LOGGER_INFO("Logger release. [{}]", name());
    _pimpl.reset();
}

Logger::Logger(std::string_view name) : _pimpl(std::make_unique<Impl>(name)) {}

Logger::Logger(std::string_view name, const std::shared_ptr<Sink>& sink) : Logger(name, {sink}) {}

Logger::Logger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks)
    : Logger(name, sinks.begin(), sinks.end())
{
}

Logger::Logger(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks)
    : Logger(name, sinks.begin(), sinks.end())
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
