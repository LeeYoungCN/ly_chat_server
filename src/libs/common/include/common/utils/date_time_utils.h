/**
 * @file date_time_utils.h
 * @brief 时间工具库核心接口声明，提供时间获取、转换、格式化及日期名称查询功能
 * @details 跨平台支持Windows/Linux/macOS，封装底层系统差异，提供统一时间处理接口
 * @author 自动生成
 * @version 1.0.0
 * @date 2025-07-27
 * @copyright Copyright (c) 2025
 */

#ifndef COMMON_UTILS_DATE_TIME_UTILS_H
#define COMMON_UTILS_DATE_TIME_UTILS_H
#include <ctime>
#include <string>
#include <string_view>

#include "common/constants/date_time_constants.h"
#include "common/types/date_time_types.h"

namespace common::utils::date_time {
/**
 * @brief 获取当前系统时间的毫秒级时间戳
 * @details 以Unix纪元（1970-01-01 00:00:00 UTC）为基准，返回当前时间的毫秒数
 * @return common::types::date_time::Timestamp 毫秒级时间戳
 * @note 跨平台实现：Windows使用GetSystemTimeAsFileTime，Linux/macOS使用std::chrono
 */
common::types::date_time::Timestamp GetCurrentTimestamp();

/**
 * @brief 获取当前系统时间的时间组件
 * @details 直接返回分解后的时间信息（年、月、日等），等价于Timestamp2Component(GetCurrentTimestamp())
 * @return common::types::date_time::TimeComponent 当前时间的分解结构
 */
common::types::date_time::TimeComponent GetCurrentTimeComponent();

/**
 * @brief 将毫秒级时间戳转换为本地时间的时间组件
 * @param[in] timestamp 待转换的毫秒级时间戳（Unix纪元基准）
 * @return common::types::date_time::TimeComponent 转换后的时间组件
 *         失败时返回所有字段为0的结构
 */
common::types::date_time::TimeComponent LocalTimeComponent(const common::types::date_time::Timestamp& timestamp);

/**
 * @brief 将毫秒级时间戳转换为UTC时间的时间组件
 * @param[in] timestamp 待转换的毫秒级时间戳（Unix纪元基准）
 * @return common::types::date_time::TimeComponent 转换后的时间组件
 *         失败时返回所有字段为0的结构
 */
common::types::date_time::TimeComponent UtcTimeComponent(const common::types::date_time::Timestamp& timestamp);

/**
 * @brief 获取月份的完整英文名称
 * @param[in] month 月份（1-12，1表示一月，12表示十二月）
 * @return std::string_view 完整月份名称（如"January"）
 *         若参数超出范围，返回空字符串视图
 */
std::string_view GetMonthFullName(uint32_t month);

/**
 * @brief 获取月份的缩写英文名称
 * @param[in] month 月份（1-12）
 * @return std::string_view 缩写月份名称（如"Jan"）
 *         若参数超出范围，返回空字符串视图
 */
std::string_view GetMonthAbbrName(uint32_t month);

/**
 * @brief 获取星期的完整英文名称
 * @param[in] weekday 星期（0-6，0表示星期日，6表示星期六）
 * @return std::string_view 完整星期名称（如"Monday"）
 *         若参数超出范围，返回空字符串视图
 */
std::string_view GetWeekdayFullName(uint32_t weekday);

/**
 * @brief 获取星期的缩写英文名称
 * @param[in] weekday 星期（0-6）
 * @return std::string_view 缩写星期名称（如"Mon"）
 *         若参数超出范围，返回空字符串视图
 */
std::string_view GetWeekdayAbbrName(uint32_t weekday);

/**
 * @brief 将时间戳按指定格式转换为字符串
 * @param[in] timestamp 待格式化的毫秒级时间戳
 * @param[in] format 格式字符串（支持占位符，如%Y表示4位年份）
 *                   默认值：common::constants::date_time::DEFAULT_TIME_FMT（"%Y-%m-%d %H:%M:%S"）
 * @return std::string 格式化后的时间字符串
 *         若格式化失败或参数无效，返回空字符串
 */
std::string FormatTimeString(common::types::date_time::Timestamp timestamp,
                             const std::string_view& format = common::constants::date_time::DEFAULT_TIME_FMT);

/**
 * @brief 将时间组件按指定格式转换为字符串
 * @param[in] timeComp 待格式化的时间组件
 * @param[in] format 格式字符串（支持占位符，如%Y表示4位年份）
 *                   默认值：common::constants::date_time::DEFAULT_TIME_FMT（"%Y-%m-%d %H:%M:%S"）
 * @return std::string 格式化后的时间字符串
 *         若格式化失败或参数无效，返回空字符串
 */
std::string FormatTimeString(const common::types::date_time::TimeComponent& timeComp,
                             const std::string_view& format = common::constants::date_time::DEFAULT_TIME_FMT);

/**
 * @brief 将时间戳按指定格式写入字符缓冲区（高性能）
 * @details 直接写入用户提供的缓冲区，减少内存分配，适合高频调用场景
 * @param[out] buffer 目标缓冲区（需提前分配内存，不可为nullptr）
 * @param[in] bufferSize 缓冲区大小（字节），建议不小于256
 * @param[in] timestamp 待格式化的毫秒级时间戳
 * @param[in] format 格式字符串（支持占位符）
 *                   默认值：common::constants::date_time::DEFAULT_TIME_FMT（"%Y-%m-%d %H:%M:%S"）
 * @return size_t 成功写入的字符数（不含终止符'\0'）
 *         若失败（缓冲区无效/空间不足/格式错误），返回0
 */
size_t FormatTimeBuffer(char* buffer, size_t bufferSize, common::types::date_time::Timestamp timestamp,
                        const std::string_view& format = common::constants::date_time::DEFAULT_TIME_FMT);

/**
 * @brief 将时间组件按指定格式写入字符缓冲区（高性能）
 * @details 直接写入用户提供的缓冲区，减少内存分配，适合高频调用场景
 * @param[out] buffer 目标缓冲区（需提前分配内存，不可为nullptr）
 * @param[in] bufferSize 缓冲区大小（字节），建议不小于256
 * @param[in] timeComp 待格式化的时间组件
 * @param[in] format 格式字符串（支持占位符）
 *                   默认值：common::constants::date_time::DEFAULT_TIME_FMT（"%Y-%m-%d %H:%M:%S"）
 * @return size_t 成功写入的字符数（不含终止符'\0'）
 *         若失败（缓冲区无效/空间不足/格式错误），返回0
 */
size_t FormatTimeBuffer(char* buffer, size_t bufferSize, const common::types::date_time::TimeComponent& timeComp,
                        const std::string_view& format = common::constants::date_time::DEFAULT_TIME_FMT);
}  // namespace common::utils::date_time
#endif  // COMMON_UTILS_DATE_TIME_UTILS_H
