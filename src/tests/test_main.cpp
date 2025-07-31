#include <cstdio>

#include "common/debug/debug_log.h"
#include "common/debug/working_env.h"
#include "common/types/logging_types.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    common::SetDebugLogLevel(common::types::logging::LogLevel::DEBUG);
    printf("Running main() from %s\n", __FILE__);
    common::debug::working_env::ShowWorkingEnv();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
