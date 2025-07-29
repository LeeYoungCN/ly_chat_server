/**
 * @file date_time_utils_string.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <string_view>

#include "common/constants/date_time_constants.h"
#include "common/debug/debug_log.h"
#include "common/utils/date_time_utils.h"

namespace {
// 星期英文名称（全程）
// 索引0-6对应：星期日-星期六
const std::array<std::string_view, 7> WEEKDAY_FULL_NAMES = {
    "Sunday",     // 星期日
    "Monday",     // 星期一
    "Tuesday",    // 星期二
    "Wednesday",  // 星期三
    "Thursday",   // 星期四
    "Friday",     // 星期五
    "Saturday"    // 星期六
};

// 星期英文名称（简称）
// 索引0-6对应：星期日-星期六
const std::array<std::string_view, 7> WEEKDAY_ABBR_NAMES = {
    "Sun",  // 星期日
    "Mon",  // 星期一
    "Tue",  // 星期二
    "Wed",  // 星期三
    "Thu",  // 星期四
    "Fri",  // 星期五
    "Sat"   // 星期六
};

// 月份英文名称（全程）
// 索引0-11对应：一月-十二月
const std::array<std::string_view, 12> MONTH_FULL_NAMES = {
    "January",    // 一月
    "February",   // 二月
    "March",      // 三月
    "April",      // 四月
    "May",        // 五月
    "June",       // 六月
    "July",       // 七月
    "August",     // 八月
    "September",  // 九月
    "October",    // 十月
    "November",   // 十一月
    "December"    // 十二月
};

// 月份英文名称（简称）
// 索引0-11对应：一月-十二月
const std::array<std::string_view, 12> MONTH_ABBR_NAMES = {
    "Jan",  // 一月
    "Feb",  // 二月
    "Mar",  // 三月
    "Apr",  // 四月
    "May",  // 五月
    "Jun",  // 六月
    "Jul",  // 七月
    "Aug",  // 八月
    "Sep",  // 九月
    "Oct",  // 十月
    "Nov",  // 十一月
    "Dec"   // 十二月
};

}  // namespace

namespace common::utils::date_time {

using namespace ::common::constants::date_time;
using namespace ::common::types::date_time;

std::string_view GetMonthFullName(uint32_t month)
{
    if (month < MIN_MONTH || month > MAX_MONTH) {
        COMMON_LOG_ERR("Month out of range [%u, %u]. weekday: %u.", MIN_MONTH, MAX_MONTH, month);
        return "";
    }
    return MONTH_FULL_NAMES.at(month - MIN_MONTH);
}

std::string_view GetMonthAbbrName(uint32_t month)
{
    if (month < MIN_MONTH || month > MAX_MONTH) {
        COMMON_LOG_ERR("Month out of range [%u, %u]. weekday: %u.", MIN_MONTH, MAX_MONTH, month);
        return "";
    }
    return MONTH_ABBR_NAMES.at(month - MIN_MONTH);
}

std::string_view GetWeekdayFullName(uint32_t weekday)
{
    if (weekday < MIN_WEEK_DAY || weekday > MAX_WEEK_DAY) {
        COMMON_LOG_ERR("Weekday out of range [%u, %u]. weekday: %u.", MIN_WEEK_DAY, MAX_WEEK_DAY, weekday);
        return "";
    }
    return WEEKDAY_FULL_NAMES.at(weekday);
}

std::string_view GetWeekdayAbbrName(uint32_t weekday)
{
    if (weekday < MIN_WEEK_DAY || weekday > MAX_WEEK_DAY) {
        COMMON_LOG_ERR("Weekday out of range [%u, %u]. weekday: %u.", MIN_WEEK_DAY, MAX_WEEK_DAY, weekday);
        return "";
    }
    return WEEKDAY_ABBR_NAMES.at(weekday);
}

std::string FormatTimeString(TimeStamp timeStamp, const std::string_view& format)
{
    auto timeComp = LocalTimeComponent(timeStamp);
    return FormatTimeString(timeComp, format);
}

std::string FormatTimeString(const TimeComponent& timeComp, const std::string_view& format)
{
    std::string timeString("", MAX_TIME_STR_LEN);
    size_t len = FormatTimeBuffer(timeString.data(), timeString.capacity(), timeComp, format);

    if (len <= 0 || len > timeString.capacity()) {
        COMMON_LOG_ERR("Time format failed or buffer overflow. len: %llu.", len);
        len = 0;
    }
    timeString.resize(len);  // resize仅修改字符串的size属性，不拷贝数据

    return timeString;
}

size_t FormatTimeBuffer(char* buffer, size_t bufferSize, TimeStamp timeStamp, const std::string_view& format)
{
    auto timeComp = LocalTimeComponent(timeStamp);
    return FormatTimeBuffer(buffer, bufferSize, timeComp, format);
}

size_t FormatTimeBuffer(char* buffer, size_t bufferSize, const TimeComponent& timeComp, const std::string_view& format)
{
    if (buffer == nullptr || bufferSize == 0) {
        COMMON_LOG_ERR("Invalid param!");
        return 0;
    }
    size_t formatIdx = 0;
    size_t bufferIdx = 0;

    //  lambda函数：将数字按指定长度写入缓冲区（自动补前导零）
    //  @param number    待写入的数字（如月份1 → 01）
    //  @param numberLen 数字固定长度（如2位、3位）
    //  @return 成功返回true；缓冲区不足返回false
    auto insertDateTimeNumber = [&](uint32_t number, size_t numberLen) -> bool {
        uint32_t tmp = number;
        if (bufferIdx + numberLen >= bufferSize) {
            COMMON_LOG_ERR("Failed to insert number: %lu", number);
            return false;
        }

        size_t currIdx = bufferIdx + numberLen;
        while (currIdx > bufferIdx) {
            buffer[currIdx - 1] = static_cast<char>(tmp % 10 + '0');
            tmp = tmp / 10;
            currIdx--;
        }
        bufferIdx += numberLen;
        return true;
    };

    auto insertString = [&](const std::string_view& name) -> bool {
        std::string_view insertName = name.empty() ? "?" : name;

        if (bufferIdx + insertName.length() >= bufferSize) {
            COMMON_LOG_ERR("Failed to insert string: %s", insertName.data());
            return false;
        }

        std::memcpy(buffer + bufferIdx, insertName.data(), insertName.size());
        bufferIdx += insertName.size();
        return true;
    };

    for (; bufferIdx < bufferSize && formatIdx < format.length(); formatIdx++) {
        if (format[formatIdx] != '%') {
            buffer[bufferIdx++] = format[formatIdx];
            continue;
        }
        formatIdx++;
        if (formatIdx >= format.length()) {
            buffer[bufferIdx++] = '%';
            break;
        }
        bool success = true;
        switch (format[formatIdx]) {
            case 'Y':
                success = insertDateTimeNumber(timeComp.year, 4);
                break;
            case 'y':
                success = insertDateTimeNumber(timeComp.year, 2);
                break;
            case 'm':
                success = insertDateTimeNumber(timeComp.month, 2);
                break;
            case 'd':
                success = insertDateTimeNumber(timeComp.day, 2);
                break;
            case 'H':
                success = insertDateTimeNumber(timeComp.hour, 2);
                break;
            case 'M':
                success = insertDateTimeNumber(timeComp.minute, 2);
                break;
            case 'S':
                success = insertDateTimeNumber(timeComp.second, 2);
                break;
            case 'B':  // 完整月份名称
                success = insertString(GetMonthFullName(timeComp.month));
                break;
            case 'b':  // 缩写月份名称
            case 'h':  // 缩写月份名称
                success = insertString(GetMonthAbbrName(timeComp.month));
                break;
            case 'A':  // 完整星期名称
                success = insertString(GetWeekdayFullName(timeComp.wday));
                break;
            case 'a':  // 缩写星期名称
                success = insertString(GetWeekdayAbbrName(timeComp.wday));
                break;
            case '%':
                buffer[bufferIdx++] = '%';
                break;
            case '3':
                if (formatIdx + 1 < format.length() && format[formatIdx + 1] == 'f') {
                    formatIdx++;
                    success = insertDateTimeNumber(timeComp.millis, 3);
                } else {
                    success = insertString(std::string_view(format.data() + formatIdx - 1, 1));
                }
                break;
            default:
                success = insertString(std::string_view(format.data() + formatIdx - 1, 1));
                break;
        }
        if (!success) {
            break;
        }
    }
    if (formatIdx < format.length() || bufferIdx >= bufferSize) {
        COMMON_LOG_ERR("Incomplete format processing (remaining: %s)", format.data() + formatIdx);
        bufferIdx = 0;
    }

    buffer[bufferIdx] = '\0';

    return bufferIdx;
}
}  // namespace common::utils::date_time
