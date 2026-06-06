#ifndef TEST_LOGGING_TEST_UTILS_COMMON_H
#define TEST_LOGGING_TEST_UTILS_COMMON_H

#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "logging/log_level.h"

namespace test::test_logging {
const std::vector<logging::LogLevel> LOG_LEVELS = {logging::LogLevel::TRACE,
                                                   logging::LogLevel::DEBUG,
                                                   logging::LogLevel::INFO,
                                                   logging::LogLevel::WARN,
                                                   logging::LogLevel::ERR,
                                                   logging::LogLevel::FATAL,
                                                   logging::LogLevel::OFF};

std::string get_logger_name(testing::TestInfo* test_info);

std::string get_log_dir();

}  // namespace test::test_logging
#endif  // TEST_LOGGING_TEST_UTILS_COMMON_H
