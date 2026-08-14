#pragma once
#ifndef TEST_LOGGING_TEST_UTILS_BASE_TEST_SINK_H
#define TEST_LOGGING_TEST_UTILS_BASE_TEST_SINK_H

#include <atomic>
#include <memory>
#include <mutex>
#include <string_view>

#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"
#include "logging/log_msg.h"
#include "logging/sinks/sink.h"

using namespace logging;

namespace test::test_logging {
class BaseTestSink : public logging::Sink {
public:
    BaseTestSink() = default;
    virtual ~BaseTestSink();

    void log(const LogMsg& logMsg) override;
    void flush() override;

    void set_pattern(std::string_view pattern) override;
    void set_formatter(std::unique_ptr<logging::Formatter> formatter) override;

    [[nodiscard]] bool should_log(logging::LogLevel level) const override;
    void set_level(logging::LogLevel level) override;
    [[nodiscard]] logging::LogLevel level() const override;

protected:
    explicit BaseTestSink(std::string_view parameter);
    virtual void log_it(const LogMsg& logMsg) = 0;
    virtual void flush_it() = 0;

protected:
    std::atomic<logging::LogLevel> _level{logging::LogLevel::INFO};
    std::unique_ptr<logging::Formatter> _formatter{std::make_unique<logging::PatternFormatter>()};
    std::mutex _sinkMtx;
    std::string _paramStr{"unknown"};
};
}  // namespace test::test_logging

#endif  // TEST_LOGGING_TEST_UTILS_BASE_TEST_SINK_H
