#ifndef TEST_LOGGING_TEST_UTILS_COMMON_H
#define TEST_LOGGING_TEST_UTILS_COMMON_H
#include <vector>

#include "logging/log_level.h"

namespace test::test_logging {
const std::vector<logging::LogLevel> LOG_LEVELS = {logging::LogLevel::DEBUG,
                                                   logging::LogLevel::INFO,
                                                   logging::LogLevel::WARN,
                                                   logging::LogLevel::ERROR,
                                                   logging::LogLevel::FATAL,
                                                   logging::LogLevel::OFF};
}

#endif  // TEST_LOGGING_TEST_UTILS_COMMON_H
