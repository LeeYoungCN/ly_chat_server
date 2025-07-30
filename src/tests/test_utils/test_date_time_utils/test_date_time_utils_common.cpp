#include "test_date_time_utils_common.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <cstdint>
#include <ctime>
#include <string_view>

#include "common/constants/date_time_constants.h"
#include "common/types/date_time_types.h"
#include "common/utils/date_time_utils.h"
#include "gtest/gtest.h"

namespace test::test_utils::test_date_time_utils {

using namespace common::constants::date_time;
using namespace common::types::date_time;
using namespace common::utils::date_time;

tm TimestampToTimeInfo(Timestamp ts)
{
    auto timer = static_cast<std::time_t>(ts / MILLIS_PER_SECOND);
    std::tm timeInfo{};

#ifdef _WIN32
    localtime_s(&timeInfo, &timer);
#else
    localtime_r(&timer, &timeInfo);
#endif
    return timeInfo;
}

void CompareString(const common::types::date_time::TimeComponent& timeComp, std::string stdTimeStr,
                   const std::string& utilsTimeStr)
{
    char buffer[4];  // 3位数字 + 终止符
    snprintf(buffer, sizeof(buffer), "%03d", timeComp.millis);
    std::string three_digit = buffer;
    std::string placeHolder(MILLIS_PLACEHOLDER);
    size_t pos = 0;
    // 替换所有%3f占位符
    while ((pos = stdTimeStr.find(placeHolder, pos)) != std::string::npos) {
        stdTimeStr.replace(pos, placeHolder.length(), three_digit);
        pos += three_digit.length();  // 跳过已替换部分，避免重复替换
    }
    EXPECT_EQ(stdTimeStr, utilsTimeStr);
}

void TestTimeBuffer(const std::string_view& format, uint32_t length, common::types::date_time::Timestamp timestamp)
{
    auto timeComp = LocalTimeComponent(timestamp);
    auto timeInfo = TimestampToTimeInfo(timestamp);
    TestTimeBuffer(format, length, timeInfo, timeComp);
}

void TestTimeBuffer(const std::string_view& format, uint32_t length, std::tm timeInfo,
                    const common::types::date_time::TimeComponent& timeComp)
{
    char bufferUtils[MAX_TIME_STR_LEN] = {'\0'};
    auto sizeUtils = common::utils::date_time::FormatTimeBuffer(bufferUtils, length, timeComp, format);

    char bufferStd[MAX_TIME_STR_LEN] = {'\0'};
    auto sizeStd = std::strftime(bufferStd, length, format.data(), &timeInfo);

    EXPECT_EQ(sizeStd, sizeUtils);

    if (sizeUtils > 0) {
        CompareString(timeComp, bufferStd, bufferUtils);
    }
}

void TestTimeString(const std::string_view& format, common::types::date_time::Timestamp timestamp)
{
    auto timeComp = LocalTimeComponent(timestamp);
    auto timeInfo = TimestampToTimeInfo(timestamp);
    TestTimeString(format, timeInfo, timeComp);
}

void TestTimeString(const std::string_view& format, std::tm timeInfo,
                    const common::types::date_time::TimeComponent& timeComp)
{
    auto utilsStr = common::utils::date_time::FormatTimeString(timeComp, format);
    std::stringstream ss;
    ss << std::put_time(&timeInfo, format.data());

    EXPECT_EQ(ss.str().size(), utilsStr.size());

    if (utilsStr.size() > 0) {
        CompareString(timeComp, ss.str(), utilsStr);
    }
}

}  // namespace test::test_utils::test_date_time_utils
