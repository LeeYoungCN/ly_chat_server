#include "common/utils/date_time_utils.h"

#ifdef _WIN32
#include <windows.h>
#endif  // _WIN32

#include <chrono>
#include <cstdint>
#include <ctime>

#include "common/constants/date_time_constants.h"
#include "common/debug/debug_log.h"
#include "common/types/date_time_types.h"

namespace utils::date_time {

using namespace ::common::constants::date_time;
using namespace ::common::types::date_time;

TimeStamp GetCurrentTimeStamp()
{
#ifdef _WIN32
    FILETIME ft;
    // 获取当前系统时间，以FILETIME格式存储（从Windows纪元1601-01-01 00:00:00开始的100纳秒间隔数）
    GetSystemTimeAsFileTime(&ft);

    // 将FILETIME的高低位 DWORD 合并为64位无符号整数，得到完整的100纳秒单位时间戳
    constexpr int FILETIME_HIGH_SHIFT_BITS = 32;  // FILETIME高32位左移位数
    uint64_t file_time = (static_cast<uint64_t>(ft.dwHighDateTime) << FILETIME_HIGH_SHIFT_BITS) | ft.dwLowDateTime;

    // 转换为Unix时间戳（毫秒级）：
    // 1. 先将100纳秒单位转换为毫秒（除以10000，因1毫秒=10000×100纳秒）
    // 2. 减去Windows纪元到Unix纪元（1970-01-01 00:00:00）的毫秒差值，得到标准Unix时间戳
    constexpr uint64_t HUNDRED_NANOSECONDS_PER_MILLISECOND = 10000;
    return static_cast<TimeStamp>((file_time / HUNDRED_NANOSECONDS_PER_MILLISECOND)  // 转换为毫秒
                                  - WINDOWS_EPOCH_TO_UNIX_EPOCH_MS                   // 校正到Unix纪元
    );
#else
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::duration ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return static_cast<TimeStamp>(ms.count());
#endif
}

TimeComponent GetCurrentTimeComp()
{
    return TimeStamp2Component(GetCurrentTimeStamp());
}

bool SafeLocalTime(const time_t* timer, struct tm* tp)
{
    if (tp == nullptr) {
        COMMON_LOG_ERR("SafeLocalTime: Output tm pointer is nullptr.");
        return false;
    }
    time_t timeVal =
        (timer != nullptr) ? *timer : std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

#ifdef _WIN32
    // Windows 使用 localtime_s
    auto err = localtime_s(tp, &timeVal);
    COMMON_LOG_COND((err == 0), "[Windows] Get local time. err = {}", err);
    return (err == 0);
#else
    // Linux/macOS 使用 localtime_r
    auto rst = localtime_r(&timeVal, tp);
    COMMON_LOG_COND((rst != nullptr), "[Linux] Get local time.");
    return (rst != nullptr);
#endif
}

TimeComponent TimeStamp2Component(const TimeStamp& timeStamp)
{
    auto timer = static_cast<std::time_t>(timeStamp / MILLIS_PER_SECOND);
    auto millis = static_cast<int32_t>(timeStamp % MILLIS_PER_SECOND);

    std::tm timeInfo{};
    SafeLocalTime(&timer, &timeInfo);

    TimeComponent timeComp;

    timeComp.year = static_cast<uint32_t>(TIME_COMP_START_YEAR + timeInfo.tm_year);
    timeComp.month = static_cast<uint32_t>(TIME_COMP_START_MONTH + timeInfo.tm_mon);
    timeComp.day = static_cast<uint32_t>(timeInfo.tm_mday);
    timeComp.hour = static_cast<uint32_t>(timeInfo.tm_hour);
    timeComp.minute = static_cast<uint32_t>(timeInfo.tm_min);
    timeComp.second = static_cast<uint32_t>(timeInfo.tm_sec);
    timeComp.millis = static_cast<uint32_t>(millis);
    timeComp.wday = static_cast<uint32_t>(timeInfo.tm_wday);
    timeComp.yday = static_cast<uint32_t>(timeInfo.tm_yday);
    return timeComp;
}

}  // namespace utils::date_time
