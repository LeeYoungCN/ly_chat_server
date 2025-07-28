
#ifndef TEST_UTILS_TEST_DATE_TIME_UTILS_COMMON_H
#define TEST_UTILS_TEST_DATE_TIME_UTILS_COMMON_H

#include <cstdint>
#include <ctime>
#include <string_view>

#include "common/types/date_time_types.h"

namespace test::test_utils::test_date_time_utils {

std::tm TimeStampToTimeInfo(::common::types::date_time::TimeStamp ts);

void TestTimeBuffer(const std::string_view& format, uint32_t length, common::types::date_time::TimeStamp timeStamp);

void TestTimeBuffer(const std::string_view& format, uint32_t length, std::tm timeInfo,
                    const common::types::date_time::TimeComponent& timeComp);

void TestTimeString(const std::string_view& format, common::types::date_time::TimeStamp timeStamp);

void TestTimeString(const std::string_view& format, std::tm timeInfo,
                    const common::types::date_time::TimeComponent& timeComp);
}  // namespace test::test_utils::date_time_test
#endif  // TEST_UTILS_TEST_DATE_TIME_UTILS_COMMON_H
