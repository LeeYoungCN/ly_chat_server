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
#include "logging/appender.h"

#include <filesystem>

#include "common/constants/date_time_constants.h"
#include "common/constants/filesystem_constants.h"
#include "common/utils/date_time_utils.h"
#include "logging/logging_types.h"

namespace logging {

void Appender::append(const logging::LogRecord& record)
{
    if (record.level < m_filterLevel) {
        return;
    }
    std::unique_lock<std::mutex> lock(m_appendMtx);
    flush(defaultFormatLog(record));
}

std::string Appender::defaultFormatLog(const logging::LogRecord& record)
{
    std::stringstream logStream;
    logStream << "[" << common::utils::date_time::FormatTimeString(record.time) << "] "
              << "[" << common::types::logLevelToStr(record.level) << "] "
              << "[" << record.threadId << "] " << "[" << std::filesystem::path(record.file).filename().string() << ":"
              << record.line << "] " << "[" << record.loggerName << "] " << record.message;
    return logStream.str();
}

void Appender::setName(const std::string& name)
{
    m_name = name;
}

std::string Appender::getName()
{
    return m_name;
}
}  // namespace logging
