#include <cstdint>
#include <cstdio>

#include "common/debug/log.h"
#include "common/macros.h"
#include "common/types/log_level.h"
#include "gtest/gtest.h"
#include "logging/log_macros.h"
#include "logging/logger.h"
#include "logging/logger_factory.h"
#include "utils/filesystem_utils.h"

int main(int argc, char *argv[])
{
    std::string processPath = utils::GetProcessPath();
    COMMON_LOG_INFO("Running main() from {}", __FILE__);
    COMMON_LOG_INFO("Test runtime: {}", processPath.c_str());
    std::string logFolder = utils::RelativeToAbsolutePath("./logs", utils::GetProcessDirectory());
    std::string logFile = logFolder + "/test.log";
    utils::RemoveDirectory(logFolder);
    LOGGER()->log(common::types::LogLevel::ERROR, __FILE__, __LINE__, __func__, "Test log");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
