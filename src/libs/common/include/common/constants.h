/**
 * @file constants.h
 * @author your name (you@domain.com)
 * @brief 公共常量
 * @version 0.1
 * @date 2025-07-23
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef CONATENTS_H
#define CONATENTS_H

#include <cstdint>

#include "common/types/time_common.h"

namespace common {
constexpr uint32_t MAX_PATH_LEN = 1024;

constexpr int32_t TIME_COMP_START_YEAR = 1900;
constexpr int32_t TIME_COMP_START_MONTH = 1;

// 1970-01-01 00:00:00 UTC对应的时间戳（毫秒）
constexpr common::types::TimeStamp UNIX_EPOCH_MS = 0;

// 纪元年份（Unix时间戳起始年份）
// EPOCH（通常译为 “纪元” 或 “时间起点”）指的是时间戳计数的起始点，是用于统一时间计算的基准时间。
constexpr int EPOCH_YEAR = 1970;
constexpr int EPOCH_MONTH = 1;   // 1月（注意：部分API使用0-based月份）
constexpr int EPOCH_DAY = 1;     // 1日
constexpr int EPOCH_HOUR = 0;    // 0时
constexpr int EPOCH_MINUTE = 0;  // 0分
constexpr int EPOCH_SECOND = 0;  // 0秒

// Windows FILETIME 与 Unix 时间戳的转换常量
// FILETIME从1601-01-01开始，与Unix纪元的差值（毫秒）
// 计算方式: (1970-1601)年的总毫秒数 = 369年 * 365天 + 89个闰年天数
// 369*365=134,685 + 89=134,774天 * 86,400秒/天 = 11,644,473,600秒
// 转换为毫秒: 11,644,473,600 * 1000 = 11,644,473,600,000
constexpr uint64_t WINDOWS_EPOCH_TO_UNIX_EPOCH_MS = 11644473600000ULL;

// 增加纪元时间戳常量，便于直接使用
constexpr int64_t UNIX_EPOCH_TIMESTAMP_SEC = 0;       // 纪元时间的秒级时间戳
constexpr int64_t UNIX_EPOCH_TIMESTAMP_MS = 0;        // 纪元时间的毫秒级时间戳
constexpr int64_t WINDOWS_EPOCH_TIMESTAMP_100NS = 0;  // Windows纪元的100纳秒级时间戳

constexpr const char* DEFAULT_TIME_FMT = "%Y-%m-%d %H:%M:%S.%3f";
constexpr const char* MILLIS_PLACEHOLDER = "%3f";
constexpr uint32_t MAX_TIME_STR_LEN = 256;

/**
 * @brief 1秒包含的毫秒数
 *
 * 用于时间戳单位转换（毫秒 → 秒），例如：
 * - 毫秒时间戳转秒级时间戳：ts_ms / MILLIS_PER_SECOND
 * - 秒级时间戳转毫秒：ts_sec * MILLIS_PER_SECOND
 *
 * 替代直接使用硬编码的1000，增强代码可读性和可维护性
 */
constexpr int64_t MILLIS_PER_SECOND = 1000;

/**
 * @brief 1秒包含的微秒数（扩展定义，常用于更高精度转换）
 */
constexpr int64_t MICROS_PER_SECOND = 1000000;

/**
 * @brief 1毫秒包含的微秒数
 */
constexpr int64_t MICROS_PER_MILLI = 1000;

}  // namespace common
#endif  // CONATENTS_H
