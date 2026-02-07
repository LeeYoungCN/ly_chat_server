#include <initializer_list>
#include <memory>

#include "common/debug/debug_log.h"
#include "common/debug/working_env.h"
#include "gtest/gtest.h"
#include "logging/async_logger.h"
#include "logging/details/common.h"
#include "logging/details/log_source.h"
#include "logging/details/log_thread_pool.h"
#include "logging/logging.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/stdout_sink.h"

int main(int argc, char *argv[])
{
    set_debug_log_level(DebugLevel::DEBUG_LVL_INFO);

    std::shared_ptr pool = std::make_shared<logging::details::LogThreadPool>(1024, 5);

    auto stdoutSink = std::make_shared<logging::StdoutSink>();
    auto fileSink = std::make_shared<logging::BasicFileSink>();

    std::initializer_list<std::shared_ptr<logging::Sink>> list = {stdoutSink, fileSink};

    auto logger = std::make_shared<logging::AsyncLogger>("async_logger", list, pool);
    logging::set_root_logger(std::move(logger));
    logging::info(LOG_SRC_LOCAL, "Running main() from {}", __FILE__);
    common::debug::working_env::ShowWorkingEnv();
    testing::InitGoogleTest(&argc, argv);
    auto rst =  RUN_ALL_TESTS();
    logging::info(LOG_SRC_LOCAL, "Run all tests finish. Result: {}.", rst);
    logging::shut_down();
    return rst;
}
