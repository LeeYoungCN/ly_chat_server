#include "common/debug/debug_log.h"
#include "common/debug/debug_logger.h"
#include "common/debug/working_env.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    set_debug_log_level(DebugLevel::DEBUG_LVL_DEBUG);
    DEBUG_LOGGER_INFO("Running main() from {}", __FILE__);

#ifdef DEBUG_MODE
    common::debug::working_env::ShowWorkingEnv();
#endif
    testing::InitGoogleTest(&argc, argv);
    auto rst = RUN_ALL_TESTS();

    DEBUG_LOGGER_INFO("Run all tests finish. Result: {}.", rst);

    return rst;
}
