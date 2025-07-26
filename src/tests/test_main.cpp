#include "common/debug/debug_log.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    std::string processPath = utils::fs::GetProcessPath();
    COMMON_LOG_INFO("Running main() from {}", __FILE__);
    COMMON_LOG_INFO("Test runtime: {}", processPath.c_str());
    std::string logFolder = utils::fs::RelativeToAbsolutePath("./logs", utils::fs::GetProcessDir());
    std::string logFile = logFolder + "/test.log";
    utils::fs::RemoveDir(logFolder);

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
