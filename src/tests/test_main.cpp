#include <cstdio>
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    printf("Running main() from %s\n", __FILE__);
    printf("Test runtime: %s\n", common::utils::filesystem::GetProcessPath().c_str());
    printf("Current working directory: %s\n", common::utils::filesystem::GetCurrentWorkingDirectory().c_str());
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
