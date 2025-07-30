
#include <cstdint>
#include <string_view>

#include "common/constants/date_time_constants.h"
#include "common/utils/date_time_utils.h"
#include "gtest/gtest.h"

using namespace common::constants::date_time;
using namespace common::types::date_time;

namespace test::test_utils::test_date_time_utils {
TEST(GetMothNames, case)
{
    for (uint32_t i = 0; i < 20; i++) {
        auto len1 = common::utils::date_time::GetMonthFullName(i).length();
        auto len2 = common::utils::date_time::GetMonthAbbrName(i).length();
        if (i >= MIN_MONTH && i <= MAX_MONTH) {
            EXPECT_GT(len1, 0);
            EXPECT_GT(len2, 0);
        } else {
            EXPECT_EQ(len1, 0);
            EXPECT_EQ(len2, 0);
        }
    }
}

TEST(GetWeekdayNames, case)
{
    for (uint32_t i = 0; i < 20; i++) {
        auto len1 = common::utils::date_time::GetWeekdayFullName(i).length();
        auto len2 = common::utils::date_time::GetWeekdayAbbrName(i).length();
        if (i >= MIN_WEEK_DAY && i <= MAX_WEEK_DAY) {
            EXPECT_GT(len1, 0);
            EXPECT_GT(len2, 0);
        } else {
            EXPECT_EQ(len1, 0);
            EXPECT_EQ(len2, 0);
        }
    }
}
}  // namespace test::test_utils::test_date_time_utils
