/**
 * @file test_logging.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <memory>

#include "gtest/gtest.h"
#include "logging/appender/console_appender.h"
#include "logging/details/log_source.h"
#include "logging/logger.h"

TEST(TestLogging, case)
{
    auto logger = logging::Logger("test_logger");
    auto appender = std::make_shared<logging::ConsoleAppender>("test");
    logger.addAppender(appender);
    logger.error(LOG_SOURCE, "test log");
}
