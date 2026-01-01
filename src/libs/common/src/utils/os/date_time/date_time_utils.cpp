#include "common/utils/date_time_utils.h"

#include <cerrno>

#include "common/compiler/macros.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#endif  // PLATFORM_WINDOWS

#include <chrono>
#include <cstdint>
#include <ctime>

#include "common/common_error_code.h"
#include "common/constants/date_time_constants.h"
#include "common/debug/debug_log.h"
#include "common/types/date_time_types.h"
#include "common/types/error_code_types.h"
#include "common/utils/error_code_utils.h"

namespace {

using namespace ::common::constants::date_time;
using namespace ::common::types::date_time;

bool SafeLocalTime(time_t timer, tm& timeInfo)
{
#if PLATFORM_WINDOWS
    // Windows 使用 localtime_s
    auto err = localtime_s(&timeInfo, &timer);
    if (err != 0) {
        SetLastErrcode(ERR_COMM_TIMESTAMP_INVALID);
        // 特别处理负数时间戳的错误提示
        if (timer < 0) {
            DEBUG_LOG_WARN("[FAILED] localtime_s may not support negative. time: %lld, err: %d", timer, err);
        } else {
            DEBUG_LOG_ERR("[FAILED] localtime_s. time: %lld, err: %d", timer, err);
        }
        return false;
    }
#else
    // Linux/macOS 使用 localtime_r
    if (localtime_r(&timer, &timeInfo) == nullptr) {
        SetLastErrcode(ERR_COMM_TIMESTAMP_INVALID);
        DEBUG_LOG_ERR("[FAILED] localtime_r. time: %lld, errno: %d", timer, errno);
        return false;
    }
#endif
    SetLastErrcode(ERR_COMM_SUCCESS);
    return true;
}

bool SafeGmtime(time_t timer, tm& timeInfo)
{
#if PLATFORM_WINDOWS
    // Windows下使用gmtime_s，增加负数时间戳检查
    errno_t err = gmtime_s(&timeInfo, &timer);
    if (err != 0) {
        SetLastErrcode(ERR_COMM_TIMESTAMP_INVALID);
        // 针对负数时间戳的错误做特殊提示
        if (timer < 0) {
            DEBUG_LOG_WARN("[FAILED] gmtime_s may not support negative time: %lld, err: %d", timer, err);
        } else {
            DEBUG_LOG_ERR("[FAILED] gmtime_s time: %lld, err: %d", timer, err);
        }
        return false;
    }
#else
    // Linux/macOS使用gmtime_r（对负数时间戳支持更完善）
    if (gmtime_r(&timer, &timeInfo) == nullptr) {
        SetLastErrcode(ERR_COMM_TIMESTAMP_INVALID);
        DEBUG_LOG_ERR("[FAILED] gmtime_r. time: %lld, errno: %d", timer, errno);
        return false;
    }
#endif
    SetLastErrcode(ERR_COMM_SUCCESS);
    return true;
}

void ConvertTmToTimeComp(const std::tm timeInfo, int32_t millis, common::types::date_time::TimeComponent& timeComp)
{
    timeComp.year = static_cast<uint32_t>(TIME_COMP_START_YEAR + timeInfo.tm_year);
    timeComp.month = static_cast<uint32_t>(TIME_COMP_START_MONTH + timeInfo.tm_mon);
    timeComp.day = static_cast<uint32_t>(timeInfo.tm_mday);
    timeComp.hour = static_cast<uint32_t>(timeInfo.tm_hour);
    timeComp.minute = static_cast<uint32_t>(timeInfo.tm_min);
    timeComp.second = static_cast<uint32_t>(timeInfo.tm_sec);
    timeComp.millis = static_cast<uint32_t>(millis);
    timeComp.wday = static_cast<uint32_t>(timeInfo.tm_wday);
    timeComp.yday = static_cast<uint32_t>(timeInfo.tm_yday);
}
}  // namespace

namespace common::utils::date_time {

using namespace ::common::constants::date_time;
using namespace ::common::types::date_time;

TimestampMs GetCurrentTimestampMs()
{
    SetLastErrcode(ERR_COMM_SUCCESS);
#if PLATFORM_WINDOWS
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
    return static_cast<TimestampMs>((file_time / HUNDRED_NANOSECONDS_PER_MILLISECOND)  // 转换为毫秒
                                    - WINDOWS_EPOCH_TO_UNIX_EPOCH_MS                   // 校正到Unix纪元
    );
#else
    std::chrono::time_point now = std::chrono::system_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return static_cast<TimestampMs>(ms.count());
#endif
}

TimeComponent GetCurrentTimeComponent()
{
    return TimeStampMs2Component(GetCurrentTimestampMs());
}

TimeComponent LocalTimeComponent(const TimestampMs& timestamp)
{
    return TimeStampMs2Component(timestamp, TimeZone::LOCAL);
}

TimeComponent UtcTimeComponent(const TimestampMs& timestamp)
{
    return TimeStampMs2Component(timestamp, TimeZone::UTC);
}

TimeComponent TimeStampMs2Component(TimestampMs timestamp, TimeZone timeZone)
{
    auto timer = static_cast<std::time_t>(timestamp / MILLIS_PER_SECOND);
    auto millis = static_cast<int32_t>(timestamp % MILLIS_PER_SECOND);

    std::tm timeInfo{};
    TimeComponent timeComp{};
    bool rst = false;
    switch (timeZone) {
        case TimeZone::UTC:
            rst = SafeGmtime(timer, timeInfo);
            break;
        case TimeZone::LOCAL:
        default:
            rst = SafeLocalTime(timer, timeInfo);
    }

    if (!rst) {
        DEBUG_LOG_ERR("[FAILED] Get time info, zone: %s, message: %s.", GetTimeZoneString(timeZone), GetLastErrorStr());
    } else {
        ConvertTmToTimeComp(timeInfo, millis, timeComp);
        SetLastErrcode(ERR_COMM_SUCCESS);
        DEBUG_LOG_DBG(
            "[SUCCESS] Get time info, zone: %s, message: %s.", GetTimeZoneString(timeZone), GetLastErrorStr());
    }
    return timeComp;
}

}  // namespace common::utils::date_time
