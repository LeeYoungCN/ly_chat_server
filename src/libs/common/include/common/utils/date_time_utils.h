/**
 * @file date_time_utils.h
 * @author your name (you@domain.com)
 * @brief   时间日期相关函数封装
 * @version 0.1
 * @date 2025-07-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef common_UTILS_TIME_UTILS_H
#define common_UTILS_TIME_UTILS_H
#include <ctime>
#include <string>
#include <string_view>

#include "common/constants/date_time_constants.h"
#include "common/types/date_time_types.h"

namespace utils::date_time {
/**
 * @brief 获取当前系统时间的Unix时间戳（毫秒级）
 *
 * 该函数跨平台实现了当前时间到Unix时间戳的转换，返回自1970年1月1日00:00:00 UTC以来的毫秒数。
 * Windows平台使用系统API获取高精度时间，类Unix平台（Linux/macOS）使用C++11标准库实现，
 * 保证在不同操作系统下的一致性和兼容性。
 *
 * @return ::common::types::date_time::TimeStamp 以毫秒为单位的Unix时间戳
 *
 * @note Unix时间戳起点：1970-01-01 00:00:00 UTC
 * @note Windows系统时间起点：1601-01-01 00:00:00 UTC（需转换偏移量）
 */
::common::types::date_time::TimeStamp GetCurrentTimeStamp();

/**
 * @brief 获取当前的时间分量
 *
 * @return ::common::types::date_time::TimeStamp
 */
::common::types::date_time::TimeComponent GetCurrentTimeComp();

/**
 * @brief 线程安全的本地时间转换函数（跨平台实现）
 *
 * 将时间戳（time_t）转换为本地时间的 tm 结构体，
 * 内部根据平台自动选择线程安全的系统函数，避免多线程竞争问题。
 *
 * @param timer 输入参数，指向待转换的时间戳（time_t 类型）
 *              若为 nullptr，默认转换当前时间（time(nullptr)）
 * @param tp 输出参数，指向存储转换结果的 tm 结构体
 * @return bool 转换成功返回 true，失败返回 false
 *
 * @note 1. 线程安全：Windows 用 localtime_s，Linux/macOS 用 localtime_r
 *       2. 错误处理：失败时会触发断言（DEBUG 模式）并记录错误日志
 *       3. 参数校验：对输入指针进行有效性检查，避免空指针访问
 */
bool SafeLocalTime(const time_t* timer, struct tm* tp);

/**
 * @brief 将时间戳(TimeStamp)转换为时间组件结构体(TimeComponent)
 *
 * 该函数将微秒级时间戳转换为包含年、月、日、时、分、秒、毫秒等详细信息的时间组件，
 * 内部使用线程安全的本地时间转换函数，并补充时间戳中的毫秒部分。
 *
 * @param[in] ts 输入的时间戳(TimeStamp类型)，单位为微秒(μs)
 *
 * @return ::common::types::date_time::TimeComponent 转换后的时间组件结构体，包含：
 *         - year: 年份（完整4位年份，如2024）
 *         - month: 月份（1-12）
 *         - day: 日期（1-31）
 *         - hour: 小时（0-23）
 *         - minute: 分钟（0-59）
 *         - second: 秒（0-59）
 *         - millis: 毫秒（0-999）
 *         - wday: 星期几（0-6，0表示周日，取决于系统实现）
 *         - yday: 年内天数（0-365）
 */
::common::types::date_time::TimeComponent TimeStamp2Component(const ::common::types::date_time::TimeStamp& timeStamp);

/**
 * @brief 格式化时间组件为std::string字符串，基于FormatTimeBuffer实现
 *
 * 内部调用FormatTimeBuffer将时间格式化到预分配的字符串缓冲区，
 * 自动处理缓冲区大小和字符串长度调整，支持与FormatTimeBuffer相同的格式说明符。
 *
 * @param[in] timeStamp 时间戳(毫秒)
 * @param[in] format    格式字符串，支持的格式说明符同FormatTimeBuffer（如%Y、%m、%3f等）
 *
 * @return std::string 成功时返回格式化后的时间字符串；
 *                     失败时返回空字符串
 *
 * @note 1. 内部预分配缓冲区大小为::common::MAX_TIME_STR_LEN，避免频繁扩容；
 *       2. 依赖FormatTimeBuffer的格式化逻辑，格式解析规则完全一致；
 *       3. 若格式化失败（如格式字符串无效），会触发断言（用于调试阶段快速发现问题）。
 */
std::string FormatTimeString(::common::types::date_time::TimeStamp timeStamp,
                             const std::string_view& format = ::common::constants::date_time::DEFAULT_TIME_FMT);

/**
 * @brief 格式化时间组件为std::string字符串，基于FormatTimeBuffer实现
 *
 * 内部调用FormatTimeBuffer将时间格式化到预分配的字符串缓冲区，
 * 自动处理缓冲区大小和字符串长度调整，支持与FormatTimeBuffer相同的格式说明符。
 *
 * @param[in] timeComp 时间组件结构体，包含年、月、日、时、分、秒、毫秒信息
 * @param[in] format   格式字符串，支持的格式说明符同FormatTimeBuffer（如%Y、%m、%3f等）
 *
 * @return std::string 成功时返回格式化后的时间字符串；
 *                     失败时返回空字符串
 *
 * @note 1. 内部预分配缓冲区大小为::common::MAX_TIME_STR_LEN，避免频繁扩容；
 *       2. 依赖FormatTimeBuffer的格式化逻辑，格式解析规则完全一致；
 *       3. 若格式化失败（如格式字符串无效），会触发断言（用于调试阶段快速发现问题）。
 */
std::string FormatTimeString(const ::common::types::date_time::TimeComponent& timeComp,
                             const std::string_view& format = ::common::constants::date_time::DEFAULT_TIME_FMT);

/**
 * @brief 格式化时间组件到字符缓冲区，支持自定义时间格式
 *
 * 功能类似于标准库 strftime，但针对自定义的 TimeComponent 结构体进行格式化，
 * 支持的格式说明符包括：%Y(年，4位)、%y(年，2位)、%m(月，2位)、%d(日，2位)、
 * %H(时，24小时制，2位)、%M(分，2位)、%S(秒，2位)、%%(转义%)、%3f(毫秒，3位)。
 * 若缓冲区空间不足或格式错误，会返回0。
 *
 * @param[out] buffer        输出缓冲区，用于存储格式化后的时间字符串
 * @param[in]  bufferSize    缓冲区大小（字节），需包含终止符'\0'的空间
 * @param[in]  timeStamp     时间戳(毫秒)
 * @param[in]  format        格式字符串，包含普通字符和格式说明符（以%开头）
 *
 * @return size_t 成功时返回写入缓冲区的字符数（不含终止符'\0'）；
 *                失败时返回0（如参数无效、缓冲区不足、格式解析失败）
 *
 * @note 1. 若格式字符串未完全解析或缓冲区填满，会清空缓冲区并返回0；
 *       2. 数字格式化时会自动补前导零（如月份5会格式化为"05"）；
 *       3. %3f是扩展格式，需连续出现（如"%.3f"无效，必须是"%3f"）。
 */
size_t FormatTimeBuffer(char* buffer, size_t bufferSize, ::common::types::date_time::TimeStamp timeStamp,
                        const std::string_view& format = ::common::constants::date_time::DEFAULT_TIME_FMT);

/**
 * @brief 格式化时间组件到字符缓冲区，支持自定义时间格式
 *
 * 功能类似于标准库 strftime，但针对自定义的 TimeComponent 结构体进行格式化，
 * 支持的格式说明符包括：%Y(年，4位)、%y(年，2位)、%m(月，2位)、%d(日，2位)、
 * %H(时，24小时制，2位)、%M(分，2位)、%S(秒，2位)、%%(转义%)、%3f(毫秒，3位)。
 * 若缓冲区空间不足或格式错误，会返回0。
 *
 * @param[out] buffer        输出缓冲区，用于存储格式化后的时间字符串
 * @param[in]  bufferSize    缓冲区大小（字节），需包含终止符'\0'的空间
 * @param[in]  timeComp      时间组件结构体，包含年、月、日、时、分、秒、毫秒信息
 * @param[in]  format        格式字符串，包含普通字符和格式说明符（以%开头）
 *
 * @return size_t 成功时返回写入缓冲区的字符数（不含终止符'\0'）；
 *                失败时返回0（如参数无效、缓冲区不足、格式解析失败）
 *
 * @note 1. 若格式字符串未完全解析或缓冲区填满，会清空缓冲区并返回0；
 *       2. 数字格式化时会自动补前导零（如月份5会格式化为"05"）；
 *       3. %3f是扩展格式，需连续出现（如"%.3f"无效，必须是"%3f"）。
 */
size_t FormatTimeBuffer(char* buffer, size_t bufferSize, const ::common::types::date_time::TimeComponent& timeComp,
                        const std::string_view& format = ::common::constants::date_time::DEFAULT_TIME_FMT);
}  // namespace utils::date_time
#endif  // ::common_UTILS_TIME_UTILS_H
