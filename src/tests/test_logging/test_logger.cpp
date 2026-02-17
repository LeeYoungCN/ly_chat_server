#include <cstdint>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "logging/details/common.h"
#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"
#include "logging/logger.h"
#include "test_utils/common.h"
#include "test_utils/log_content_buffer_sink.h"

using namespace test::test_logging;
using namespace logging;
using namespace logging::details;

namespace test::test_logging {

class TestLogger : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override {};

protected:
    std::shared_ptr<Logger> _logger;
    std::shared_ptr<LogContentBuffer> _sink = std::make_shared<LogContentBuffer>();
};

TEST_F(TestLogger, create_single_sink)
{
    const std::string name = test_info_->name();
    _logger = std::make_shared<Logger>(name, _sink);
    EXPECT_EQ(_sink.use_count(), 2);
    EXPECT_EQ(_logger->name(), name);
    EXPECT_EQ(_logger->sinks().size(), 1);
    auto *sinkPtr = reinterpret_cast<LogContentBuffer *>(_logger->sinks()[0].get());
    EXPECT_EQ(sinkPtr->buffer().size(), 0);
}

TEST_F(TestLogger, create_initializer_list)
{
    const std::string name = test_info_->name();
    auto sinks = std::initializer_list<std::shared_ptr<Sink>>{_sink, _sink, _sink};
    _logger = std::make_shared<Logger>(name, sinks);
    EXPECT_EQ(_logger->name(), name);
    EXPECT_EQ(_logger->sinks().size(), sinks.size());
    EXPECT_EQ(_sink.use_count(), 2 * sinks.size() + 1);
}

TEST_F(TestLogger, create_vector)
{
    const std::string name = test_info_->name();
    auto sinks = std::vector<std::shared_ptr<Sink>>{_sink, _sink};
    sinks.push_back(_sink);
    _logger = std::make_shared<Logger>(name, sinks);
    EXPECT_EQ(_logger->name(), name);
    EXPECT_EQ(_logger->sinks().size(), sinks.size());
    EXPECT_EQ(_sink.use_count(), 2 * sinks.size() + 1);
}

TEST_F(TestLogger, log_level)
{
    const std::string name = test_info_->name();
    _logger = std::make_shared<Logger>(name, _sink);

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

TEST_F(TestLogger, flush_level)
{
    const std::string name = test_info_->name();
    _logger = std::make_shared<Logger>(name, _sink);

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

TEST_F(TestLogger, log_log)
{
    const std::string name = test_info_->name();
    _sink->set_level(LogLevel::DEBUG);
    _logger = std::make_shared<Logger>(name, _sink);
    for (auto filterLevel : LOG_LEVELS) {
        _logger->set_level(filterLevel);
        for (auto logLevel : LOG_LEVELS) {
            _logger->log(LOG_SRC_LOCAL, logLevel, "test");
        }
        if (filterLevel != LogLevel::OFF) {
            EXPECT_EQ(_sink->buffer().size(), LogLevel::FATAL - _logger->level() + 1)
                << log_level_to_string(filterLevel);
        } else {
            EXPECT_EQ(_sink->buffer().size(), 0);
        }
        _sink->clear();
    }
}

TEST_F(TestLogger, log_flush)
{
    const std::string name = test_info_->name();
    _sink->set_level(LogLevel::DEBUG);
    _logger = std::make_shared<Logger>(name, _sink);
    constexpr uint32_t MAX_ITEM_CNT = 100;
    for (uint32_t i = 0; i < MAX_ITEM_CNT; ++i) {
        _logger->error(LOG_SRC_LOCAL, i);
        EXPECT_EQ(_sink->buffer().size(), i + 1);
        EXPECT_EQ(_sink->disk().size(), 0);
    }
    _logger->flush();
    EXPECT_EQ(_sink->buffer().size(), 0);
    EXPECT_EQ(_sink->disk().size(), MAX_ITEM_CNT);
}

TEST_F(TestLogger, log_flush_on)
{
    const std::string name = test_info_->name();
    _sink->set_level(LogLevel::DEBUG);
    _logger = std::make_shared<Logger>(name, _sink);
    _logger->set_level(LogLevel::DEBUG);

    for (auto flushLevel : LOG_LEVELS) {
        // 设施刷新等级
        _logger->flush_on(flushLevel);
        for (uint32_t i = 0; i < LOG_LEVELS.size(); ++i) {
            LogLevel level = LOG_LEVELS[i];
            if (level == LogLevel::OFF) {
                break;
            }
            _logger->log(LOG_SRC_LOCAL, level, i);
            if (_logger->flush_level() == LogLevel::OFF || level < _logger->flush_level()) {
                EXPECT_EQ(_sink->buffer().size(), i + 1);
                EXPECT_EQ(_sink->disk().size(), 0);
            } else {
                EXPECT_EQ(_sink->buffer().size(), 0);
                EXPECT_EQ(_sink->disk().size(), i + 1);
            }
        }
        _sink->clear();
    }
}

TEST_F(TestLogger, log_function)
{
    const std::string name = test_info_->name();
    _sink->set_level(LogLevel::DEBUG);
    _logger = std::make_shared<Logger>(name, _sink);
    _logger->set_level(LogLevel::DEBUG);
    for (uint32_t i = 0; i < 100; ++i) {
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
}

TEST_F(TestLogger, set_pattern)
{
    const std::string name = test_info_->name();
    _logger = std::make_shared<Logger>(name, _sink);
    _logger->set_pattern("%v");
    for (uint32_t i = 0; i < 100; i++) {
        _logger->error(i);
        EXPECT_EQ(std::to_string(i), _sink->buffer()[i]);
    }
}

TEST_F(TestLogger, set_formatter)
{
    const std::string name = test_info_->name();
    _sink->set_level(LogLevel::DEBUG);
    _logger = std::make_shared<Logger>(name, _sink);
    _logger->set_level(LogLevel::DEBUG);
    std::unique_ptr<Formatter> formatter = std::make_unique<PatternFormatter>("%v");
    _logger->set_formatter(formatter);
    for (uint32_t i = 0; i < 100; i++) {
        _logger->error(i);
        EXPECT_EQ(std::to_string(i), _sink->buffer()[i]);
    }
}

}  // namespace test::test_logging
