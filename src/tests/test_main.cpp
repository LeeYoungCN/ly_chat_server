#include "common/debug/debug_log.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    std::string processPath = common::utils::filesystem::GetProcessPath();
    COMMON_LOG_INFO("Running main() from %s", __FILE__);
    COMMON_LOG_INFO("Test runtime: %s", processPath.c_str());
    std::string logFolder = common::utils::filesystem::RelativeToAbsolutePath("./logs", common::utils::filesystem::GetProcessDir());
    std::string logFile = logFolder + "/test.log";
    common::utils::filesystem::RemoveDir(logFolder);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
