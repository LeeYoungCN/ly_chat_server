#include <cstdint>
#include <cstdio>
#include <ostream>

#include "common/base/singleton_base.h"
#include "common/file_system/file_system.h"
#include "common/file_system/path.h"
#include "gtest/gtest.h"
#include "utils/logging/logger.h"

int main(int argc, char *argv[])
{
    std::string processPath = proj::GetProcessPath();
    printf("Running main() from %s\n", __FILE__);
    printf("Test runtime: %s\n", processPath.c_str());
    std::string logFolder = proj::RelativeToAbsolutePath("./logs", proj::GetProcessDirectory());
    std::string logFile = logFolder + "/test.log";
    proj::RemoveDirectory(logFolder);
    LoggerInitParams params = {.baseFileName = logFile, .logTarget = LogTarget::BOTH};
    INST(Logger).init(params);
    INST(Logger).setLogLevel(LogLevel::DEBUG);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
