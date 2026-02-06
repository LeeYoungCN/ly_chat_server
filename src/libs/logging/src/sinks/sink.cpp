#include "logging/sinks/sink.h"

#include <mutex>

#include "logging/details/common.h"
#include "logging/formatters/formatter.h"

namespace logging {
void Sink::log(const details::LogMsg& logMsg)
{
    std::lock_guard lock(_mtx);
    std::string content;
    content.reserve(LOG_CONTENT_DEFAULT_LEN);
    _formatter->format(logMsg, content);
    sink_it(content);
}

void Sink::flush()
{
    std::lock_guard lock(_mtx);
    flush_it();
}

bool Sink::should_log(LogLevel level) const
{
    return level >= _level.load();
}

void Sink::set_level(LogLevel level)
{
    _level.store(level);
};

void Sink::set_pattern(const std::string& pattern, const std::string& timePattern)
{
    set_formatter(std::make_unique<PatternFormatter>(pattern, timePattern));
}

void Sink::set_formatter(std::unique_ptr<Formatter> formatter)
{
    std::lock_guard lock(_mtx);
    _formatter = std::move(formatter);
}

}  // namespace logging
