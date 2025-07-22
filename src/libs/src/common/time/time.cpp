#include "common/time/time.h"

#include <cstdint>
#include <ctime>

namespace proj {

namespace {
constexpr uint32_t START_YEAR = 1900;
constexpr uint32_t START_MONTH = 1;
}  // namespace

TimeSt GetCurrentTime()
{
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);

    TimeSt timeSt;

    timeSt.year = START_YEAR + static_cast<uint32_t>(ltm->tm_year);
    timeSt.month = START_MONTH + static_cast<uint32_t>(ltm->tm_mon);
    timeSt.day = static_cast<uint32_t>(ltm->tm_mday);
    timeSt.hour = static_cast<uint32_t>(ltm->tm_hour);
    timeSt.minute = static_cast<uint32_t>(ltm->tm_min);
    timeSt.second = static_cast<uint32_t>(ltm->tm_sec);

    return timeSt;
}

std::string FormatCurrTimeStr(const std::string& fmt)
{
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    char buffer[80];
    std::strftime(buffer, sizeof(buffer), fmt.c_str(), ltm);
    return buffer;
}

std::string FormatTimeStr(const TimeSt& timeSt, const std::string& fmt)
{
    std::tm time = {};
    time.tm_year = static_cast<int32_t>(timeSt.year - START_YEAR);
    time.tm_mon = static_cast<int32_t>(timeSt.month - 1);
    time.tm_mday = static_cast<int32_t>(timeSt.day);
    time.tm_hour = static_cast<int32_t>(timeSt.hour);
    time.tm_min = static_cast<int32_t>(timeSt.minute);
    time.tm_sec = static_cast<int32_t>(timeSt.second);
    char buffer[80];

    std::strftime(buffer, sizeof(buffer), fmt.c_str(), &time);
    return buffer;
}
}  // namespace proj
