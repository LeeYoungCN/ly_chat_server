#include <cstdint>
#include <string_view>
#include <tuple>

#include "common/constants/date_time_constants.h"
#include "common/types/date_time_types.h"
#include "common/utils/date_time_utils.h"
#include "gtest/gtest.h"
#include "test_date_time_utils_common.h"

namespace test::test_utils::test_date_time_utils {

using TestBufferParam = std::tuple<std::string_view, int32_t>;

using namespace common::constants::date_time;
using namespace common::types::date_time;

const std::vector<std::string_view> TestMillisFormats = {"%Y-%y-%m-%d-%H:%M:%S-%A-%a-%B-%b-%h-%3f",
                                                         "%3f",
                                                         "%Y-%y-%m-%d-%H:%M:%S-%A-%a-%B-%3f-%b-%h",
                                                         "%3f%3f%3f%3f",
                                                         "%3f3f3f3f%3f"};

class TestFormatTimeBuffer : public ::testing::TestWithParam<TestBufferParam> {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override {};
};

TEST_P(TestFormatTimeBuffer, TimestampMs)
{
    constexpr int64_t MILLIS_PER_DAY = 86400000;
    const auto &format = std::get<0>(GetParam());
    auto day = std::get<1>(GetParam());
    TimestampMs const currTs = common::utils::date_time::GetCurrentTimestampMs() + day * MILLIS_PER_DAY;

    TestTimeBuffer(format, MAX_TIME_STR_LEN, currTs);
}

INSTANTIATE_TEST_SUITE_P(DAY, TestFormatTimeBuffer,
                         testing::Combine(testing::Values("%Y-%y-%m-%d %H:%M:%S %A %a %B %b"),
                                          testing::Range(0, 365, 10)));

#ifdef __linux__
INSTANTIATE_TEST_SUITE_P(Linux, TestFormatTimeBuffer,
                         testing::Combine(testing::ValuesIn(TestMillisFormats), testing::Values(0)));
#endif

class TestFormatTimeString : public ::testing::TestWithParam<std::string_view> {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override {};
    void TearDown() override {};
};

TEST_P(TestFormatTimeString, TimestampMs)
{
    const auto &format = GetParam();
    TimestampMs const currTs = common::utils::date_time::GetCurrentTimestampMs();
    TestTimeString(format, currTs);
}

INSTANTIATE_TEST_SUITE_P(FORMAT, TestFormatTimeString,
                         testing::Values(DEFAULT_TIME_FMT, "%Y-%m-%d %H:%M:%S %B", "%Y-%m-%d %H:%M:%S %b",
                                         "%Y-%m-%d %H:%M:%S %A", "%Y-%m-%d%H:%M:%S %a", "%Y%y%m%d%H%M%S%A%a%B%b",
                                         "%y-%m-%d%H:%M:%S", "%Y-%m-%d %H:%M:%S", "%Y%m%d%H%M%S", "%Y%m%d%H%M%S%%",
                                         "%%%%Y", "asdffsfsaf...."));

#ifdef _MSVC
INSTANTIATE_TEST_SUITE_P(Windows, TestFormatTimeString, testing::ValuesIn(TestMillisFormats));
#endif
}  // namespace test::test_utils::test_date_time_utils
