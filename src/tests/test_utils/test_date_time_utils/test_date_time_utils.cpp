
#include <cstdint>
#include <string_view>

#include "common/constants/date_time_constants.h"
#include "common/utils/date_time_utils.h"
#include "gtest/gtest.h"

using namespace common::date_time;

namespace test::test_utils::test_date_time_utils {
TEST(TestDateTimeUtilsMonthName, case)
{
    for (uint32_t i = 0; i < 20; i++) {
        auto len1 = common::date_time::GetMonthFullName(i).length();
        auto len2 = common::date_time::GetMonthAbbrName(i).length();
        if (i >= MIN_MONTH && i <= MAX_MONTH) {
            EXPECT_GT(len1, 0);
            EXPECT_GT(len2, 0);
        } else {
            EXPECT_EQ(len1, 0);
            EXPECT_EQ(len2, 0);
        }
    }
}

TEST(TestDateTimeUtilsWeekdayName, case)
{
    for (uint32_t i = 0; i < 20; i++) {
        auto len1 = common::date_time::GetWeekdayFullName(i).length();
        auto len2 = common::date_time::GetWeekdayAbbrName(i).length();
        if (i >= MIN_WEEKDAY && i <= MAX_WEEKDAY) {
            EXPECT_GT(len1, 0);
            EXPECT_GT(len2, 0);
        } else {
            EXPECT_EQ(len1, 0);
            EXPECT_EQ(len2, 0);
        }
    }
}
}  // namespace test::test_utils::test_date_time_utils
