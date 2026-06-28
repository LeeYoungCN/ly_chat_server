#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "common/constants/filesystem_constants.h"
#include "gtest/gtest.h"
#include "logging/details/log_source.h"
#include "logging/log_level.h"
#include "logging/logging.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/rotating_file_sink.h"
#include "test_logging_utils/common.h"
#include "utils/filesystem_utils.h"

using namespace logging;
using namespace logging::details;
using namespace utils::filesystem;
using namespace constants::filesystem;

namespace test::test_logging {
class TestRotatingFileSink : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override;
    void TearDown() override;

protected:
    std::string _dir = get_log_dir();
};

void TestRotatingFileSink::SetUp()
{
    create_dir(_dir);
}

void TestRotatingFileSink::TearDown()
{
    delete_dir(_dir);
}

TEST_F(TestRotatingFileSink, invalid_param)
{
    EXPECT_THROW(RotatingFileSink(""), std::invalid_argument);
    EXPECT_THROW(RotatingFileSink("test.log", 0, ROTATING_FILE_SINK_DEFAULT_MAX_FILES),
                 std::invalid_argument);
    EXPECT_THROW(RotatingFileSink("test.log", ROTATING_FILE_SINK_DEFAULT_MAX_FILE_SIZE, 0),
                 std::invalid_argument);
}

TEST_F(TestRotatingFileSink, find_rotating_file)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});

    // valid file
    std::vector<std::string> validFileList;
    for (uint32_t i = ROTATING_FILE_SINK_MIN_INDEX; i <= 10; ++i) {
        std::string file = logFile + "." + std::to_string(i);
        create_file(file);
        validFileList.emplace_back(file);
    }

    // invalid file
    create_file(logFile + ".00");
    create_file(logFile + ".01");
    create_file(logFile + ".");
    create_file(logFile + ".a");
    create_file(logFile + "a");

    RotatingFileSink sink(logFile);

    auto fileList = sink.get_rotating_file_list();
    EXPECT_EQ(fileList.size(), validFileList.size());
    for (uint32_t i = 0; i < validFileList.size(); ++i) {
        EXPECT_EQ(fileList[i], validFileList[i]);
    }
}

TEST_F(TestRotatingFileSink, rotate)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});
    std::string nextLogFile = logFile + ".1";

    uint32_t maxFileSize = 1024;
    RotatingFileSink sink(logFile, maxFileSize);

    sink.set_pattern("%v");

    std::string logContent = std::string(100, 'a');

    size_t logMsgSize = logContent.size() + LF_LENGTH;
    uint32_t currSize = 0;
    while (currSize < maxFileSize) {
        LogMsg logMsg(LOG_SRC_LOCAL, "noname", LogLevel::ERR, logContent);
        sink.log(logMsg);
        currSize += logMsgSize;
    }

    EXPECT_TRUE(file_exists(logFile));
    EXPECT_LT(get_file_size(logFile), maxFileSize);
    EXPECT_TRUE(file_exists(nextLogFile));
    EXPECT_LT(get_file_size(nextLogFile), maxFileSize);
    EXPECT_GT(get_file_size(nextLogFile), maxFileSize - logMsgSize);
}

TEST_F(TestRotatingFileSink, rotate_and_delete)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});

    uint32_t overflow_file_cnt = 10;
    uint32_t maxFileSize = 1024;
    uint32_t rotate_log_file = 2;

    for (uint32_t i = ROTATING_FILE_SINK_MIN_INDEX; i <= overflow_file_cnt; ++i) {
        std::string file = logFile + "." + std::to_string(i);
        create_file(file);
    }

    std::string nextLogFile = logFile + "." + std::to_string(overflow_file_cnt + rotate_log_file);

    RotatingFileSink sink(logFile, maxFileSize, 1);

    sink.set_pattern("%v");

    std::string logContent = std::string(100, 'a');

    size_t logMsgSize = logContent.size() + LF_LENGTH;
    uint32_t currSize = 0;
    while (currSize < rotate_log_file * maxFileSize) {
        LogMsg logMsg(LOG_SRC_LOCAL, "noname", LogLevel::ERR, logContent);
        sink.log(logMsg);
        currSize += logMsgSize;
    }

    EXPECT_TRUE(file_exists(logFile));
    EXPECT_LT(get_file_size(logFile), maxFileSize);
    EXPECT_TRUE(file_exists(nextLogFile));
    EXPECT_LT(get_file_size(nextLogFile), maxFileSize);
    EXPECT_GT(get_file_size(nextLogFile), maxFileSize - logMsgSize);
}

}  // namespace test::test_logging
