#include <sys/types.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

#include "common/constants/date_time_constants.h"
#include "common/types/date_time_types.h"
#include "gtest/gtest.h"
#include "logging/sinks/daily_file_sink.h"
#include "test_logging_utils/common.h"
#include "utils/date_time_utils.h"
#include "utils/filesystem_utils.h"

using namespace constants::date_time;
using namespace logging;
using namespace utils::filesystem;
using namespace utils::date_time;

namespace test::test_logging {

class TestDailyFileSink : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override;
    void TearDown() override;

protected:
    std::string _dir = get_log_dir();
};

void TestDailyFileSink::SetUp()
{
    delete_dir(_dir);
    create_dir(_dir);
}

void TestDailyFileSink::TearDown()
{
    // delete_dir(_dir);
}

TEST_F(TestDailyFileSink, create_success)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});
    DailyFileSink sink(logFile);
    EXPECT_TRUE(file_exists(sink.log_file()));
}

TEST_F(TestDailyFileSink, param_invalid)
{
    EXPECT_THROW(DailyFileSink(""), std::invalid_argument);

    EXPECT_THROW(DailyFileSink("test.log", MAX_HOUR + 1, 0, DailyFileSink::DEFAULT_MAX_FILES),
                 std::out_of_range);

    EXPECT_THROW(DailyFileSink("test.log", 0, MAX_MINUTE + 1, DailyFileSink::DEFAULT_MAX_FILES),
                 std::out_of_range);
}

TEST_F(TestDailyFileSink, find_file)
{
    std::string filenameStem = get_logger_name(test_info_);
    std::string baseFile = join_paths({_dir, filenameStem + ".log"});
    TimestampMs now = get_now_timestamp_ms();
    std::vector<std::string> validFiles;
    for (int32_t i = 10; i >= 0; --i) {
        auto timeStamp = now - i * MILLIS_PER_DAY;
        std::string file =
            join_paths({_dir, filenameStem + format_time_string(timeStamp, "_%Y%m%d") + ".log"});
        create_file(file);
        validFiles.push_back(file);
        sleep_ms(10);
    }

    create_file(baseFile);
    create_file(baseFile + ".log");
    create_file(join_paths({_dir, filenameStem + "_2026.log"}));
    create_file(join_paths({_dir, filenameStem + "_202601.log"}));
    create_file(join_paths({_dir, filenameStem + "_20260732.log"}));
    create_file(join_paths({_dir, filenameStem + "_00000000.log"}));
    create_file(join_paths({_dir, filenameStem + "_test.log"}));

    DailyFileSink sink(baseFile);
    auto fileList = sink.get_file_list();
    ASSERT_EQ(fileList.size(), validFiles.size());
    for (uint32_t i = 0; i < validFiles.size(); ++i) {
        EXPECT_EQ(validFiles[i], fileList[i]);
    }
}

}  // namespace test::test_logging
