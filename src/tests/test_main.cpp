#include <initializer_list>
#include <memory>

#include "common/debug/debug_log.h"
#include "common/debug/working_env.h"
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

    auto stdoutSink = std::make_shared<logging::StdoutSink>();
    auto fileSink = std::make_shared<logging::BasicFileSink>();

    std::initializer_list<std::shared_ptr<logging::Sink>> list = {stdoutSink, fileSink};

    auto logger = std::make_shared<logging::AsyncLogger>("AsyncLogger", list, pool);
    logger->info(LOG_SRC_LOCAL, "Running main() from {}", __FILE__);
    // LOGGING_INFO("Running main() from %s", __FILE__);
    common::debug::working_env::ShowWorkingEnv();
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
