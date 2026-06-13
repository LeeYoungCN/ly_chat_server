#include <memory>

#include "gtest/gtest.h"
#include "logging/logger.h"
#include "logging/logging.h"
#include "logging/sinks/stdout_sink.h"
#include "test_logging_utils/common.h"
#include "test_logging_utils/log_content_buffer_sink.h"

using namespace test::test_logging;
using namespace logging;
using namespace logging::details;

namespace test::test_logging {
class TestRootLogger : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override {};
};

TEST_F(TestRootLogger, create_logger)
{
    const std::string name = get_logger_name(test_info_);
    std::shared_ptr<Logger> logger = create_logger<LogContentBufferSink>(name);
    EXPECT_NE(logger, nullptr);
    EXPECT_EQ(logger->name(), name);
    EXPECT_EQ(logger->sinks().size(), 1);
    auto *sink = reinterpret_cast<LogContentBufferSink *>(logger->sinks()[0].get());
    EXPECT_EQ(sink->buffer().size(), 0);
}

TEST_F(TestRootLogger, set_root_logger)
{
    const std::string name = get_logger_name(test_info_);
    std::shared_ptr<Logger> logger = create_logger<StdoutSink>(name);
    set_root_logger(logger);
    EXPECT_EQ(root_logger()->name(), name);
    EXPECT_EQ(root_logger_raw()->name(), name);
}

TEST_F(TestRootLogger, get_root_logger)
{
    const std::string name = get_logger_name(test_info_);
    std::shared_ptr<Logger> logger = create_logger<StdoutSink>(name);
    set_root_logger(logger);
    EXPECT_EQ(root_logger()->name(), name);
    EXPECT_EQ(root_logger_raw()->name(), name);

    root_logger()->set_level(LogLevel::INFO);
    EXPECT_EQ(root_logger()->level(), LogLevel::INFO);
    EXPECT_EQ(root_logger_raw()->level(), LogLevel::INFO);
    EXPECT_TRUE(should_log(LogLevel::INFO));
    EXPECT_FALSE(should_log(LogLevel::DEBUG));
}

TEST_F(TestRootLogger, root_logger_function)
{
    const std::string name = get_logger_name(test_info_);
    std::shared_ptr<Logger> logger = create_logger<StdoutSink>(name);
    set_root_logger(logger);
    root_logger()->set_level(LogLevel::TRACE);
    trace(LOG_SRC_LOCAL, "trace {}", name);
    debug(LOG_SRC_LOCAL, "debug {}", name);
    info(LOG_SRC_LOCAL, "info {}", name);
    warn(LOG_SRC_LOCAL, "warn {}", name);
    error(LOG_SRC_LOCAL, "error {}", name);
    fatal(LOG_SRC_LOCAL, "fatal {}", name);

    trace("trace {}", name);
    debug("debug {}", name);
    info("info {}", name);
    warn("warn {}", name);
    error("error {}", name);
    fatal("fatal {}", name);

    trace(LOG_SRC_LOCAL, name);
    debug(LOG_SRC_LOCAL, name);
    info(LOG_SRC_LOCAL, name);
    warn(LOG_SRC_LOCAL, name);
    error(LOG_SRC_LOCAL, name);
    fatal(LOG_SRC_LOCAL, name);

    trace(name);
    debug(name);
    info(name);
    warn(name);
    error(name);
    fatal(name);
}

}  // namespace test::test_logging
