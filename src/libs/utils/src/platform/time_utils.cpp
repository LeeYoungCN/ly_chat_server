#include "utils/time_utils.h"

#include <chrono>
#include <cstdint>
#include <ctime>
#include <iomanip>

#include "common/constants.h"
#include "common/debug/log.h"
#include "common/types/time_common.h"

namespace utils::time {
common::types::TimeStamp GetCurrentTimeStamp()
{
#ifdef _WIN32
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);
    uint64_t file_time = (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
    return static_cast<Timestamp>((file_time / 10000ULL) - common::WINDOWS_EPOCH_TO_UNIX_EPOCH_MS);
#else
    std::chrono::time_point now = std::chrono::system_clock::now();
    std::chrono::duration ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
    return static_cast<common::types::TimeStamp>(ms.count());
#endif
}

common::types::TimeComponent GetCurrentTimeComp()
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

common::types::TimeComponent TimeStamp2Component(const common::types::TimeStamp& ts)
{
    auto timer = static_cast<std::time_t>(ts / common::MICROS_PER_MILLI);
    auto millis = static_cast<int32_t>(ts % common::MICROS_PER_MILLI);

    std::tm timeInfo{};
    SafeLocalTime(&timer, &timeInfo);

    common::types::TimeComponent timeComp;

    timeComp.year = common::TIME_COMP_START_YEAR + timeInfo.tm_year;
    timeComp.month = common::TIME_COMP_START_MONTH + timeInfo.tm_mon;
    timeComp.day = timeInfo.tm_mday;
    timeComp.hour = timeInfo.tm_hour;
    timeComp.minute = timeInfo.tm_min;
    timeComp.second = timeInfo.tm_sec;
    timeComp.millis = millis;
    return timeComp;
}

std::string TimeStamp2String(common::types::TimeStamp ts, const std::string& fmt)
{
    auto timer = static_cast<std::time_t>(ts / common::MICROS_PER_MILLI);
    auto ms = static_cast<int32_t>(ts % common::MICROS_PER_MILLI);

    std::tm timeInfo{};
    SafeLocalTime(&timer, &timeInfo);


    char time_buf[common::MAX_TIME_STR_LEN] = {'\0'};
    if (strftime(time_buf, sizeof(time_buf), fmt.c_str(), &timeInfo) == 0) {
        return "";  // 格式化失败
    }
    std::string result(time_buf);

    auto ms_pos = result.find(common::MILLIS_PLACEHOLDER);

    if (ms_pos != std::string::npos) {
        std::stringstream ss;
        ss << std::setw(3) << std::setfill('0') << ms;
        result.replace(ms_pos, 3, ss.str());
    }
    return result;
}
}  // namespace utils::time
