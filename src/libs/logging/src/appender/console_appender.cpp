/**
 * @file console_appender.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "logging/appender/console_appender.h"

#include <iostream>

namespace logging {
void ConsoleAppender::flush(const std::string& message)
{
    std::cout << message << std::endl;
}
}  // namespace logging
