#include <memory>

#include "common/debug/debug_log.h"
#include "common/debug/working_env.h"
#include "common/utils/filesystem_utils.h"
#include "common/utils/process_utils.h"
#include "gtest/gtest.h"
#include "logging/async_logger.h"
#include "logging/details/log_source.h"
#include "logging/details/log_task_scheduler.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/stdout_sink.h"

int main(int argc, char *argv[])
{
    set_debug_log_level(DebugLevel::DEBUG_LVL_INFO);

    std::shared_ptr pool = std::make_shared<logging::details::LogTaskScheduler>(1, 1024);
    // logging::AsyncLogger logger(pool);
    auto logger = std::make_shared<logging::AsyncLogger>(pool);

    std::string logFile = common::filesystem::JoinPaths(
        {common::process::GetProcessDirectory(), "logs", common::process::GetProcessFileName() + ".log"});
    logger->add_sink(std::make_shared<logging::StdoutSink>());
    logger->add_sink(std::make_shared<logging::BasicFileSink>(logFile));

    logger->info(LOG_SRC_LOCAL, "Running main() from {}", __FILE__);
    // LOGGING_INFO("Running main() from %s", __FILE__);
    common::debug::working_env::ShowWorkingEnv();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
