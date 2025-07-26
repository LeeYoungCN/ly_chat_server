
#include <stdint.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <string_view>

#include "common/constants/date_time_constants.h"
#include "common/debug/assert.h"
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

namespace utils::date_time {

using namespace ::common::constants::date_time;
using namespace ::common::types::date_time;

std::string_view GetMonthFullNames(uint32_t month)
{
    return MONTH_FULL_NAMES.at(month);
}

std::string FormatTimeString(TimeStamp timeStamp, const std::string_view& format)
{
    std::string timeString;
    timeString.reserve(MAX_TIME_STR_LEN);
    auto len = FormatTimeBuffer(timeString.data(), timeString.capacity(), timeStamp, format);
    COMMON_ASSERT_MSG((len > 0), "Format time str fail.");
    timeString.resize(len);
    return timeString;
}

std::string FormatTimeString(const TimeComponent& timeComp, const std::string_view& format)
{
    std::string timeString;
    timeString.reserve(MAX_TIME_STR_LEN);
    auto len = FormatTimeBuffer(timeString.data(), timeString.capacity(), timeComp, format);
    COMMON_ASSERT_MSG((len > 0), "Format time str fail.");
    timeString.resize(len);
    return timeString;
}

size_t FormatTimeBuffer(char* buffer, size_t bufferSize, TimeStamp timeStamp, const std::string_view& format)
{
    auto timeComp = TimeStamp2Component(timeStamp);
    return FormatTimeBuffer(buffer, bufferSize, timeComp, format);
}

size_t FormatTimeBuffer(char* buffer, size_t bufferSize, const TimeComponent& timeComp, const std::string_view& format)
{
    if (buffer == nullptr || bufferSize == 0) {
        COMMON_LOG_ERR("Invalid param!");
        return 0;
    }
    size_t formatIndex = 0;
    size_t bufferIndex = 0;

    //  lambda函数：将数字按指定长度写入缓冲区（自动补前导零）
    //  @param number    待写入的数字（如月份1 → 01）
    //  @param numberLen 数字固定长度（如2位、3位）
    //  @return 成功返回true；缓冲区不足返回false
    auto insertNumber = [&](uint32_t number, size_t numberLen) -> bool {
        uint32_t tmp = number;
        if (bufferIndex + numberLen >= bufferSize) {
            COMMON_LOG_ERR("Failed to insert number: {}", number);
            return false;
        }

        size_t currIdx = bufferIndex + numberLen;
        while (currIdx > bufferIndex) {
            buffer[currIdx - 1] = static_cast<char>(tmp % 10 + '0');
            tmp = tmp / 10;
            currIdx--;
        }
        bufferIndex += numberLen;
        return true;
    };

    auto insertString = [&](const std::string_view& string) -> bool {
        if (bufferIndex + string.length() >= bufferSize) {
            COMMON_LOG_ERR("Failed to insert string: {}", string);
            return false;
        }
        for (const auto& c : string) {
            buffer[bufferIndex++] = c;
        }
        return true;
    };

    for (; bufferIndex < bufferSize && formatIndex < format.length(); formatIndex++) {
        if (format[formatIndex] != '%') {
            buffer[bufferIndex++] = format[formatIndex];
            continue;
        }
        formatIndex++;
        if (formatIndex >= format.length()) {
            buffer[bufferIndex++] = '%';
            break;
        }
        bool formatResult = true;
        switch (format[formatIndex]) {
            case 'Y':
                formatResult = insertNumber(timeComp.year, 4);
                break;
            case 'y':
                formatResult = insertNumber(timeComp.year, 2);
                break;
            case 'm':
                formatResult = insertNumber(timeComp.month, 2);
                break;
            case 'd':
                formatResult = insertNumber(timeComp.day, 2);
                break;
            case 'H':
                formatResult = insertNumber(timeComp.hour, 2);
                break;
            case 'M':
                formatResult = insertNumber(timeComp.minute, 2);
                break;
            case 'S':
                formatResult = insertNumber(timeComp.second, 2);
                break;
            case 'B':  // 完整月份名称
                formatResult = insertString(MONTH_FULL_NAMES.at(timeComp.month - 1));
                break;
            case 'b':  // 缩写月份名称
            case 'h':  // 缩写月份名称
                formatResult = insertString(MONTH_ABBR_NAMES.at(timeComp.month - 1));
                break;
            case 'A':  // 完整星期名称
                formatResult = insertString(WEEKDAY_FULL_NAMES.at(timeComp.wday));
                break;
            case 'a':  // 缩写星期名称
                formatResult = insertString(WEEKDAY_ABBR_NAMES.at(timeComp.wday));
                break;
            case '%':
                buffer[bufferIndex++] = '%';
                break;
            case '3':
                if (formatIndex + 1 < format.length() && format[formatIndex + 1] == 'f') {
                    formatIndex++;
                    formatResult = insertNumber(timeComp.millis, 3);
                    break;
                }
                [[fallthrough]];  // C++17特性，显式表示需要继续执行default
            default:
                if (bufferIndex + 2 < bufferSize) {
                    buffer[bufferIndex++] = '%';
                    buffer[bufferIndex++] = format[formatIndex];
                } else {
                    formatResult = false;
                }
                break;
        }
        if (!formatResult) {
            break;
        }
    }
    if (formatIndex < format.length() || bufferIndex >= bufferSize) {
        COMMON_LOG_ERR("Format fail! fmtIdx: {}, bufIdx: {}, bufSize: {}", formatIndex, bufferIndex, bufferSize);
        bufferIndex = 0;
    }

    buffer[bufferIndex] = '\0';

    return bufferIndex;
}
}  // namespace utils::date_time
