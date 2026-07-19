#include "base_test_sink.h"

#include <atomic>
#include <mutex>
#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/formatters/pattern_formatter.h"

namespace test::test_logging {
using namespace logging;

BaseTestSink::~BaseTestSink()
{
    DEBUG_LOGGER_DBG("Release Sink. {}", _paramStr);
}

BaseTestSink::BaseTestSink(std::string_view parameter) : _paramStr(parameter) {}

void BaseTestSink::log(const details::LogMsg& logMsg)
{
    std::lock_guard lock(_sinkMtx);
    log_it(logMsg);
}

void BaseTestSink::flush()
{
    std::lock_guard lock(_sinkMtx);
    flush_it();
}

bool BaseTestSink::should_log(LogLevel level) const
{
    if (level == LogLevel::OFF) {
        return false;
    }
    return level >= _level.load(std::memory_order_relaxed);
}

void BaseTestSink::set_level(LogLevel level)
{
    _level.store(level, std::memory_order_relaxed);
};

LogLevel BaseTestSink::level() const
{
    return _level.load(std::memory_order_relaxed);
}

void BaseTestSink::set_pattern(std::string_view pattern)
{
    set_formatter(std::make_unique<PatternFormatter>(pattern));
}

void BaseTestSink::set_formatter(std::unique_ptr<Formatter> formatter)
{
    std::lock_guard lock(_sinkMtx);
    _formatter = std::move(formatter);
}

}  // namespace logging
