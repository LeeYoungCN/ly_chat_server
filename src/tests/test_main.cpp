#include <initializer_list>
#include <memory>
#include <utility>
#include <vector>

#include "common/debug/debug_log.h"
#include "common/debug/working_env.h"
#include "gtest/gtest.h"
#include "logging/async_logger.h"
#include "logging/details/common.h"
#include "logging/details/task_pool.h"
#include "logging/logger.h"
#include "logging/logging.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/stdout_sink.h"

void init_logging_module()
{
    std::shared_ptr<logging::details::TaskPool> pool =
        std::make_shared<logging::details::TaskPool>(logging::THREAD_POOL_DEFAULT_CAPACITY);
    logging::register_task_pool(pool);
    std::shared_ptr<logging::Sink> stdoutSink = std::make_shared<logging::StdoutSink>();
    std::shared_ptr<logging::Sink> fileSink = std::make_shared<logging::BasicFileSink>();

    std::shared_ptr<logging::Logger> logger = std::make_shared<logging::AsyncLogger>(
        logging::ROOT_LOGGER_NAME, std::vector{std::move(stdoutSink), std::move(fileSink)}, pool);

    logging::set_root_logger(std::move(logger));
}

int main(int argc, char *argv[])
{
    set_debug_log_level(DebugLevel::DEBUG_LVL_INFO);
    init_logging_module();

    logging::info(LOG_SRC_LOCAL, "Running main() from {}", __FILE__);
#ifdef DEBUG_MODE
    common::debug::working_env::ShowWorkingEnv();
#endif
    testing::InitGoogleTest(&argc, argv);
    auto rst = RUN_ALL_TESTS();

    logging::info(LOG_SRC_LOCAL, "Run all tests finish. Result: {}.", rst);
    logging::shut_down();
    return rst;
}
