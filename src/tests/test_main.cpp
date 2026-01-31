#include <cstdio>

#include "common/debug/debug_level.h"
#include "common/debug/debug_log.h"
#include "common/debug/working_env.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    set_debug_log_level(DEBUG_LVL_INFO);
    printf("Running main() from %s\n", __FILE__);
    common::debug::working_env::ShowWorkingEnv();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
