
#include <cstdint>
#include <ctime>
#include <iomanip>
#include <tuple>

#include "common/constants/date_time_constants.h"
#include "common/types/date_time_types.h"
#include "common/utils/date_time_utils.h"
#include "gtest/gtest.h"

using TestParam = std::tuple<std::string, uint32_t, int32_t>;

using namespace common::constants::date_time;
using namespace common::types::date_time;

class TestTimeUtilsStrBuffer : public ::testing::TestWithParam<TestParam> {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override {};

    void RunTest(TimeStamp ts, const std::string &format, uint32_t length);
};

void TestTimeUtilsStrBuffer::RunTest(TimeStamp ts, const std::string &format, uint32_t length)
{
    auto timer = static_cast<std::time_t>(ts / MILLIS_PER_SECOND);
    std::tm timeInfo{};
    utils::date_time::SafeLocalTime(&timer, &timeInfo);

    char bufferUtils[MAX_TIME_STR_LEN] = {'\0'};
    auto sizeUtils = utils::date_time::FormatTimeBuffer(bufferUtils, length, ts, format);

    char bufferStd[MAX_TIME_STR_LEN] = {'\0'};
    auto sizeStd = std::strftime(bufferStd, length, format.c_str(), &timeInfo);

    std::stringstream ss;
    ss << std::put_time(&timeInfo, format.c_str());
    EXPECT_EQ(sizeStd, sizeUtils);
    if (sizeUtils > 0) {
        EXPECT_EQ(ss.str().length(), sizeUtils);
        EXPECT_STREQ(bufferStd, bufferUtils);
        EXPECT_EQ(ss.str(), std::string(bufferUtils));
    }
}

TEST_P(TestTimeUtilsStrBuffer, ALL)
{
    constexpr int64_t MILLIS_PER_DAY = 86400000;
    const auto &format = std::get<0>(GetParam());
    auto length = std::get<1>(GetParam());
    auto day = std::get<2>(GetParam());
    TimeStamp const currTs = utils::date_time::GetCurrentTimeStamp() + day * MILLIS_PER_DAY;

    RunTest(currTs, format, length);
}

INSTANTIATE_TEST_SUITE_P(DAY, TestTimeUtilsStrBuffer,
                         testing::Combine(testing::Values("%Y-%y-%m-%d %H:%M:%S %A %a %B %b"),
                                          testing::Values(MAX_TIME_STR_LEN), testing::Range(0, 365)));

INSTANTIATE_TEST_SUITE_P(FORMAT, TestTimeUtilsStrBuffer,
                         testing::Combine(testing::Values(DEFAULT_TIME_FMT, "%Y-%m-%d %H:%M:%S %B",
                                                          "%Y-%m-%d %H:%M:%S %b", "%Y-%m-%d %H:%M:%S %A",
                                                          "%Y-%m-%d%H:%M:%S %a", "%Y%y%m%d%H%M%S%A%a%B%b",
                                                          "%y-%m-%d%H:%M:%S", "%Y-%m-%d %H:%M:%S", "%Y%m%d%H%M%S",
                                                          "%Y%m%d%H%M%S%%", "%%%%Y", "asdffsfsaf...."),
                                          testing::Values(MAX_TIME_STR_LEN), testing::Values(0)));

INSTANTIATE_TEST_SUITE_P(LENGTH, TestTimeUtilsStrBuffer,
                         testing::Combine(testing::Values("%Y-%y-%m-%d-%H:%M:%S-%A-%a-%B-%b-%h"),
                                          testing::Range(1U, MAX_TIME_STR_LEN, 10U), testing::Values(0)));

#ifndef _WIN32
INSTANTIATE_TEST_SUITE_P(Linux, TestTimeUtilsStrBuffer,
                         testing::Combine(testing::Values("%", "%y-%m-%d %H:%M:%S"), testing::Values(MAX_TIME_STR_LEN),
                                          testing::Values(0)));
#endif  // _WIN32

// buffer + to_string
// 格式
// 长度
// 日期：月,日，星期
