#include <initializer_list>
#include <memory>
#include <vector>

#include "gtest/gtest.h"
#include "logging/details/common.h"
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

TEST_F(TestLogger, log_level_filter)
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

}  // namespace test::test_logging
