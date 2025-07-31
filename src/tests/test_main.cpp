#include <cstdio>

#include "common/debug/debug_log.h"
#include "common/types/logging_types.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    common::SetDebugLogLevel(common::types::logging::LogLevel::DEBUG);
    printf("Running main() from %s\n", __FILE__);
    printf("Test runtime: %s\n", common::utils::filesystem::GetProcessPath().c_str());
    printf("Current working directory: %s\n", common::utils::filesystem::GetCurrentWorkingDirectory().c_str());
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
