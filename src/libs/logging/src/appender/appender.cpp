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

#include <format>

#include "common/utils/date_time_utils.h"
#include "common/utils/filesystem_utils.h"
#include "logging/log_record.h"

namespace logging {
using namespace common::utils::date_time;
using namespace common::types::logging;
using namespace common::utils::filesystem;

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
    return std::format("[{}] [{}] [Tid: {:#x}] [{}:{}] [{}]  {}",
                       FormatTimeString(record.time, "%Y-%m-%d %H:%M:%S.%3f"),
                       logLevelToStr(record.level),
                       record.tid,
                       GetBaseName(record.source.file),
                       record.source.line,
                       record.source.func,
                       record.message);
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
