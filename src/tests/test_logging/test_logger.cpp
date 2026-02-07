#include <cstdint>
#include <memory>
#include <vector>

#include "gtest/gtest.h"
#include "logging/log_level.h"
#include "logging/logger.h"
#include "mock_sink.h"

using namespace test::test_logging;
using namespace logging;
using namespace logging::details;

namespace {
const std::vector<LogLevel> LEVELS = {LogLevel::DEBUG,
                                      LogLevel::INFO,
                                      LogLevel::WARN,
                                      LogLevel::ERROR,
                                      LogLevel::FATAL,
                                      LogLevel::OFF};
}

namespace test::test_logging {

class TestLogger : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override {};
};

TEST_F(TestLogger, create)
{
    const std::string name = "create_test";
    const uint32_t capacity = 1024;
    Logger logger = Logger(name, std::make_shared<MockSink>(capacity));
    EXPECT_EQ(logger.name(), name);
    EXPECT_EQ(logger.sinks().size(), 1);
    auto *sink = reinterpret_cast<MockSink *>(logger.sinks()[0].get());
    EXPECT_EQ(sink->capacity(), capacity);
}

TEST_F(TestLogger, set_level)
{
    const std::string name = "set_level";
    Logger logger = Logger(name, std::make_shared<MockSink>());

    for (LogLevel level : LEVELS) {
        logger.set_level(level);
        EXPECT_EQ(logger.level(), level);
        if (level != LogLevel::OFF) {
            EXPECT_TRUE(logger.should_log(level));
        } else {
            EXPECT_FALSE(logger.should_log(level));
        }
    }
}

TEST_F(TestLogger, flush_on)
{
    const std::string name = "set_level";
    Logger logger = Logger(name, std::make_shared<MockSink>());

    for (LogLevel level : LEVELS) {
        logger.flush_on(level);
        EXPECT_EQ(logger.flush_level(), level);
        if (level != LogLevel::OFF) {
            EXPECT_TRUE(logger.should_flush(level));
        } else {
            EXPECT_FALSE(logger.should_flush(level));
        }
    }
}

}  // namespace test::test_logging
