#include "gtest/gtest.h"
#include "logging/c/logging_c.h"


namespace test::test_logging {

class TestLogger : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override {};

protected:
    LoggerSt *_logger{nullptr};
    SinkSt *_sink{nullptr};
};
}  // namespace test::test_logging
