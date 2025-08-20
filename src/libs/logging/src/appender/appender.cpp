/**
 * @file appender.cpp
 * @author your name (you@domain.com)
 * @brief 日志输出器基类
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "logging/appender/appender.h"

#include "logging/details/log_record.h"

namespace logging {
using namespace common::types::logging;
using namespace logging::details;

void Appender::append(LogRecord record)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    flush(m_formatter->format(record));
}

}  // namespace logging
