#ifndef TIME_UTILS_H
#define TIME_UTILS_H
/**
 * @file time_common.h
 * @author LiYang
 * @brief 时间先关
 * @version 0.1
 * @date 2025-07-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <ctime>
#include <string>

#include "common/constants.h"
#include "common/types/time_common.h"

namespace utils::time {
/**
 * @brief 获取当前的时间戳(ms)。
 *
 * @return common::types::TimeStamp
 */
common::types::TimeStamp GetCurrentTimeStamp();

/**
 * @brief 获取当前的时间分量
 *
 * @return common::types::TimeStamp
 */
common::types::TimeComponent GetCurrentTimeComp();

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
 * @brief 时间戳转时间分量结构体。
 *
 * @param ts 时间戳
 * @return common::types::TimeComponent
 */
common::types::TimeComponent TimeStamp2Component(const common::types::TimeStamp& ts);

/**
 * @brief 将时间戳格式化为字符串
 *
 * @param timeStamp 时间戳
 * @param format 格式化
 * @return std::string
 */
std::string TimeStamp2String(common::types::TimeStamp ts, const std::string& fmt = common::DEFAULT_TIME_FMT);

}  // namespace utils::time
#endif  // TIME_UTILS_H
