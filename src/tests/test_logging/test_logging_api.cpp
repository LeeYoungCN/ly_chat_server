#include <memory>

#include "gtest/gtest.h"
#include "logging/logger.h"
#include "logging/logging.h"
#include "mock_sink.h"

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
    std::shared_ptr<Logger> logger = create_logger<MockSink>(name, 1024);
    EXPECT_NE(logger, nullptr);
    EXPECT_EQ(logger->name(), name);
    EXPECT_EQ(logger->sinks().size(), 1);
    auto* sink = reinterpret_cast<MockSink *>(logger->sinks()[0].get());
    EXPECT_EQ(sink->capacity(), 1024);
}

}  // namespace test::test_logging
