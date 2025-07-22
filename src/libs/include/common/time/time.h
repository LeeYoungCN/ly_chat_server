#ifndef PROJ_TIME_H
#define PROJ_TIME_H
/**
 * @file time.h
 * @author LiYang
 * @brief 时间先关
 * @version 0.1
 * @date 2025-07-21
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <cstdint>
#include <string>

namespace proj {
struct TimeSt {
    uint32_t year = 1970;
    uint32_t month = 1;
    uint32_t day = 1;
    uint32_t hour = 0;
    uint32_t minute = 0;
    uint32_t second = 0;
};

TimeSt GetCurrentTime();
std::string FormatCurrTimeStr(const std::string& fmt = "%Y-%m-%d %H:%M:%S");
std::string FormatTimeStr(const TimeSt& timeSt, const std::string& fmt = "%Y-%m-%d %H:%M:%S");
}  // namespace proj
#endif  // PROJ_TIME_H
