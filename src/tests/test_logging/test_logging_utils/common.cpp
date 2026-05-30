#include "common.h"

#include <format>

#include "utils/filesystem_utils.h"
#include "utils/process_utils.h"


namespace test::test_logging {
std::string get_logger_name(testing::TestInfo* test_info)
{
    return std::format("{}_{}", test_info->test_suite_name(), test_info->name());
}

std::string get_log_dir()
{
    return utils::filesystem::JoinPaths({utils::process::get_proc_directory(), "logs"});
}
}  // namespace test::test_logging
