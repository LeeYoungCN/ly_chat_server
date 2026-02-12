#include <memory>

#include "gtest/gtest.h"
#include "logging/logger.h"
#include "logging/logging.h"
#include "test_utils/log_content_buffer_sink.h"

using namespace test::test_logging;
using namespace logging;
using namespace logging::details;

namespace test::test_logging {
class TestLoggingAPI : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override {};
};

TEST_F(TestLoggingAPI, create_logger)
{
    const std::string name = "create_test";
    std::shared_ptr<Logger> logger = create_logger<LogContentBuffer>(name);
    EXPECT_NE(logger, nullptr);
    EXPECT_EQ(logger->name(), name);
    EXPECT_EQ(logger->sinks().size(), 1);
    auto *sink = reinterpret_cast<LogContentBuffer *>(logger->sinks()[0].get());
    EXPECT_EQ(sink->buffer().size(), 0);
}

}  // namespace test::test_logging
