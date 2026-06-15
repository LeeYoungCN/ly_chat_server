#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "logging/async_logger.h"
#include "logging/details/common.h"
#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"
#include "test_logging_utils/common.h"
#include "test_logging_utils/log_content_buffer_sink.h"
#include "utils/date_time_utils.h"

using namespace test::test_logging;
using namespace logging;
using namespace logging::details;

namespace test::test_logging {

class TestAsyncLogger : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override;

    void wait_for_task()
    {
        while (_taskPool->task_count() > 0) {
            utils::date_time::sleep_ms(10);
        }
    }

protected:
    std::shared_ptr<AsyncLogger> _logger;
    std::shared_ptr<LogContentBufferSink> _sink = std::make_shared<LogContentBufferSink>();
    std::shared_ptr<logging::details::TaskPool> _taskPool =
        std::make_shared<logging::details::TaskPool>(1024, 1);
};

void TestAsyncLogger::TearDown()
{
    // _logger.reset();
    // _sink.reset();
    // _taskPool.reset();
}

TEST_F(TestAsyncLogger, create_single_sink)
{
    const std::string name = get_logger_name(test_info_);
    _logger = std::make_shared<AsyncLogger>(name, _sink, _taskPool);
    EXPECT_EQ(_sink.use_count(), 2);
    EXPECT_EQ(_logger->name(), name);
    EXPECT_EQ(_logger->sinks().size(), 1);
    auto *sinkPtr = reinterpret_cast<LogContentBufferSink *>(_logger->sinks()[0].get());
    EXPECT_EQ(sinkPtr->buffer().size(), 0);
}

TEST_F(TestAsyncLogger, create_initializer_list)
{
    const std::string name = get_logger_name(test_info_);
    auto sinks = std::initializer_list<std::shared_ptr<Sink>>{_sink, _sink, _sink};
    _logger = std::make_shared<AsyncLogger>(name, sinks, _taskPool);
    EXPECT_EQ(_logger->name(), name);
    EXPECT_EQ(_logger->sinks().size(), sinks.size());
    EXPECT_EQ(_sink.use_count(), 2 * sinks.size() + 1);
}

TEST_F(TestAsyncLogger, create_vector)
{
    const std::string name = get_logger_name(test_info_);
    auto sinks = std::vector<std::shared_ptr<Sink>>{_sink, _sink};
    sinks.push_back(_sink);
    _logger = std::make_shared<AsyncLogger>(name, sinks, _taskPool);
    EXPECT_EQ(_logger->name(), name);
    EXPECT_EQ(_logger->sinks().size(), sinks.size());
    EXPECT_EQ(_sink.use_count(), 2 * sinks.size() + 1);
}

TEST_F(TestAsyncLogger, log_level)
{
    const std::string name = get_logger_name(test_info_);
    _logger = std::make_shared<AsyncLogger>(name, _sink, _taskPool);

    for (LogLevel level : LOG_LEVELS) {
        _logger->set_level(level);
        EXPECT_EQ(_logger->level(), level);
        if (level != LogLevel::OFF) {
            EXPECT_TRUE(_logger->should_log(level));
        } else {
            EXPECT_FALSE(_logger->should_log(level));
        }
    }
}

TEST_F(TestAsyncLogger, flush_level)
{
    const std::string name = get_logger_name(test_info_);
    _logger = std::make_shared<AsyncLogger>(name, _sink, _taskPool);

    for (LogLevel level : LOG_LEVELS) {
        _logger->flush_on(level);
        EXPECT_EQ(_logger->flush_level(), level);
        if (level != LogLevel::OFF) {
            EXPECT_TRUE(_logger->should_flush(level));
        } else {
            EXPECT_FALSE(_logger->should_flush(level));
        }
    }
}

TEST_F(TestAsyncLogger, log_log)
{
    const std::string name = get_logger_name(test_info_);
    _sink->set_level(LogLevel::TRACE);
    _logger = std::make_shared<AsyncLogger>(name, _sink, _taskPool);
    for (auto filterLevel : LOG_LEVELS) {
        _logger->set_level(filterLevel);
        for (auto logLevel : LOG_LEVELS) {
            _logger->log(LOG_SRC_LOCAL, logLevel, "test");
        }
        wait_for_task();
        if (filterLevel != LogLevel::OFF) {
            EXPECT_EQ(_sink->buffer().size(), LogLevel::FATAL - _logger->level() + 1)
                << log_level_to_string(filterLevel);
        } else {
            EXPECT_EQ(_sink->buffer().size(), 0);
        }
        _sink->clear();
    }
}

TEST_F(TestAsyncLogger, log_flush)
{
    const std::string name = get_logger_name(test_info_);
    _sink->set_level(LogLevel::TRACE);
    _logger = std::make_shared<AsyncLogger>(name, _sink, _taskPool);
    constexpr uint32_t MAX_ITEM_CNT = 100;
    for (uint32_t i = 0; i < MAX_ITEM_CNT; ++i) {
        _logger->error(LOG_SRC_LOCAL, i);
    }

    _logger->flush();

    wait_for_task();

    EXPECT_EQ(_sink->buffer().size(), 0);
    EXPECT_EQ(_sink->disk().size(), MAX_ITEM_CNT);
}

TEST_F(TestAsyncLogger, log_function)
{
    const std::string name = get_logger_name(test_info_);
    _sink->set_level(LogLevel::TRACE);
    _logger = std::make_shared<AsyncLogger>(name, _sink, _taskPool);
    _logger->set_level(LogLevel::TRACE);
    uint32_t logCount = 100;
    for (uint32_t i = 0; i < logCount; ++i) {
        // trace
        _logger->trace(LOG_SRC_LOCAL, "{}", i);
        _logger->trace("{}", i);
        _logger->trace(LOG_SRC_LOCAL, i);
        _logger->trace(i);
        // debug
        _logger->debug(LOG_SRC_LOCAL, "{}", i);
        _logger->debug("{}", i);
        _logger->debug(LOG_SRC_LOCAL, i);
        _logger->debug(i);
        // info
        _logger->info(LOG_SRC_LOCAL, "{}", i);
        _logger->info("{}", i);
        _logger->info(LOG_SRC_LOCAL, i);
        _logger->info(i);
        // warn
        _logger->warn(LOG_SRC_LOCAL, "{}", i);
        _logger->warn("{}", i);
        _logger->warn(LOG_SRC_LOCAL, i);
        _logger->warn(i);
        // error
        _logger->warn(LOG_SRC_LOCAL, "{}", i);
        _logger->warn("{}", i);
        _logger->warn(LOG_SRC_LOCAL, i);
        _logger->warn(i);
        // fatal
        _logger->fatal(LOG_SRC_LOCAL, "{}", i);
        _logger->fatal("{}", i);
        _logger->fatal(LOG_SRC_LOCAL, i);
        _logger->fatal(i);
    }

    _logger->flush();
    wait_for_task();
    EXPECT_EQ(_sink->buffer().size(), 0);
    EXPECT_EQ(_sink->disk().size(), logCount * (LOG_LEVELS.size() - 1) * 4);
}

TEST_F(TestAsyncLogger, set_pattern)
{
    const std::string name = get_logger_name(test_info_);
    _logger = std::make_shared<AsyncLogger>(name, _sink, _taskPool);
    _logger->set_pattern("%v");
    constexpr uint32_t logCount = 100;
    for (uint32_t i = 0; i < logCount; i++) {
        _logger->error(i);
    }
    wait_for_task();
    for (uint32_t i = 0; i < logCount; i++) {
        EXPECT_EQ(std::to_string(i), _sink->buffer()[i]);
    }
}

TEST_F(TestAsyncLogger, set_formatter)
{
    constexpr uint32_t logCount = 50;
    const std::string name = get_logger_name(test_info_);
    _sink->set_level(LogLevel::TRACE);
    _logger = std::make_shared<AsyncLogger>(name, _sink, _taskPool);
    _logger->set_level(LogLevel::TRACE);
    std::unique_ptr<Formatter> formatter = std::make_unique<PatternFormatter>("%v");
    _logger->set_formatter(formatter);
    for (uint32_t i = 0; i < logCount; i++) {
        _logger->error(i);
    }
    wait_for_task();
    for (uint32_t i = 0; i < logCount; i++) {
        EXPECT_EQ(std::to_string(i), _sink->buffer()[i]);
    }
}

}  // namespace test::test_logging
