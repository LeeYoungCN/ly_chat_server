/**
 * @file date_time_types.h
 * @author your name (you@domain.com)
 * @brief 时间戳
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#ifndef COMMON_TYPES_DATE_TIME_TYPES_H
#define COMMON_TYPES_DATE_TIME_TYPES_H

#include <cstdint>
#include <string>

namespace common::types::date_time {

// 时间戳(ms)
using Timestamp = int64_t;
using TimestampMs = int64_t;
using TimestampSec = int64_t;

using StringTimestamp = std::string;

struct ZonedTimestamp {
    Timestamp ms_since_epoch;  // 毫秒级时间戳
    int timezone_offset;       // 时区偏移（分钟，如UTC+8为480）
};

/**
 * @brief 时间分量结构体，用于表示分解后的日期和时间信息（含毫秒）
 *
 * 该结构体将时间分解为年、月、日、时、分、秒、毫秒等独立字段，
 * 方便时间的格式化、计算和展示。
 */
struct TimeComponent {
    uint32_t year = 0;    ///< 年份（4位完整年份，如2024）
    uint32_t month = 0;   ///< 月份（1-12，1表示一月，12表示十二月）
    uint32_t day = 0;     ///< 日期（1-31，当月的第几天）
    uint32_t hour = 0;    ///< 小时（0-23，24小时制）
    uint32_t minute = 0;  ///< 分钟（0-59）
    uint32_t second = 0;  ///< 秒（0-59）
    uint32_t millis = 0;  ///< 毫秒（0-999）
    uint32_t wday = 0;    ///< 星期几（0-6，具体对应关系取决于系统，通常0为星期日）
    uint32_t yday = 0;    ///< 年内天数（0-365，0表示1月1日）
};

}  // namespace common::types::date_time
#endif  // COMMON_TYPES_DATE_TIME_TYPES_H
