#include "common.h"

#include <format>

namespace test::test_logging {
std::string get_logger_name(testing::TestInfo* test_info)
{
    return std::format("{}_{}", test_info->test_suite_name(), test_info->name());
}
}  // namespace test::test_logging
