#include "common/debug/debug_log.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    std::string processPath = utils::filesystem::GetProcessPath();
    COMMON_LOG_INFO("Running main() from %s", __FILE__);
    COMMON_LOG_INFO("Test runtime: %s", processPath.c_str());
    std::string logFolder = utils::filesystem::RelativeToAbsolutePath("./logs", utils::filesystem::GetProcessDir());
    std::string logFile = logFolder + "/test.log";
    utils::filesystem::RemoveDir(logFolder);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
