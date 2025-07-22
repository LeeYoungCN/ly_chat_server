/**
 * @file time_common.h
 * @author your name (you@domain.com)
 * @brief 时间戳
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#ifndef TIMME_COMMON_H
#define TIMME_COMMON_H

#include <cstdint>
#include <string>

namespace common::types {

// 时间戳(ms)
using TimeStamp = int64_t;
using TimestampMs = int64_t;
using TimestampSec = int64_t;

using StringTimestamp = std::string;

struct ZonedTimestamp {
    int64_t ms_since_epoch;  // 毫秒级时间戳
    int timezone_offset;     // 时区偏移（分钟，如UTC+8为480）
};

/**
 * @brief 时间分量结构体，用于表示分解后的日期和时间信息（含毫秒）
 *
 * 该结构体将一个完整的时间点拆分为年、月、日、时、分、秒和毫秒七个分量，
 * 便于单独访问或处理时间的各个部分，适用于日志输出、时间计算等场景。
 * 所有字段均采用有符号32位整数，确保能覆盖合理的时间范围。
 */
struct TimeComponent {
    int32_t year = 0;    ///< 年份（完整值，如2023、1970），无固定范围但通常为公元纪年
    int32_t month = 0;   ///< 月份（1-12），1表示一月，12表示十二月
    int32_t day = 0;     ///< 日期（1-31），具体范围取决于当月天数（如二月通常28-29天）
    int32_t hour = 0;    ///< 小时（0-23），0表示午夜，23表示晚上11点
    int32_t minute = 0;  ///< 分钟（0-59）
    int32_t second = 0;  ///< 秒（0-59，闰秒场景下可能为60，但通常不处理）
    int32_t millis = 0;  ///< 毫秒（0-999），从0开始计数，每1000毫秒进位为1秒
};

}  // namespace common::types
#endif  // TIMME_COMMON_H
