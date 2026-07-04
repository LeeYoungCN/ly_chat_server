#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <vector>

#include "common/constants/filesystem_constants.h"
#include "gtest/gtest.h"
#include "logging/details/log_source.h"
#include "logging/log_level.h"
#include "logging/sinks/rotating_file_sink.h"
#include "test_logging_utils/common.h"
#include "utils/date_time_utils.h"
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

    void TestRotate(const std::string& logFile, const std::vector<uint32_t>& existFileIdxs);

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

void TestRotatingFileSink::TestRotate(const std::string& logFile,
                                      const std::vector<uint32_t>& existFileIdxs)
{
    for (const auto& idx : existFileIdxs) {
        std::string file = logFile + "." + std::to_string(idx);
        create_file(file);
        utils::date_time::sleep_ms(2);  // 确保文件修改时间不同
    }

    const uint32_t maxFileSize = 1024;
    RotatingFileSink sink(logFile, maxFileSize, RotatingFileSink::DEFAULT_MAX_FILES, true);

    sink.set_pattern("%v");

    std::string logContent = std::string(100, 'a');

    size_t logMsgSize = logContent.size() + LF_LENGTH;
    size_t currSize = 0;
    while (currSize < maxFileSize) {
        LogMsg logMsg(LOG_SRC_LOCAL, "noname", LogLevel::ERR, logContent);
        sink.log(logMsg);
        currSize += logMsgSize;
    }

    EXPECT_TRUE(file_exists(logFile));
    EXPECT_LT(get_file_size(logFile), maxFileSize);
    uint32_t nextIdx = (existFileIdxs.empty() ? 0 : existFileIdxs.back()) + 2;

    if (nextIdx > RotatingFileSink::MAX_INDEX) {
        nextIdx -= RotatingFileSink::MAX_INDEX;
    }

    std::string nextLogFile = logFile + "." + std::to_string(nextIdx);
    EXPECT_TRUE(file_exists(nextLogFile));
    EXPECT_LT(get_file_size(nextLogFile), maxFileSize);
    EXPECT_GT(get_file_size(nextLogFile), maxFileSize - logMsgSize);
}

TEST_F(TestRotatingFileSink, invalid_param)
{
    EXPECT_THROW(RotatingFileSink(""), std::invalid_argument);
    EXPECT_THROW(RotatingFileSink("test.log", 0, RotatingFileSink::DEFAULT_MAX_FILES),
                 std::invalid_argument);
    EXPECT_THROW(
        RotatingFileSink(
            "test.log", RotatingFileSink::DEFAULT_MAX_FILE_SIZE, RotatingFileSink::MAX_INDEX + 1),
        std::out_of_range);
}

TEST_F(TestRotatingFileSink, find_rotating_file)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});

    // valid file
    std::vector<std::string> validFileList;

    for (uint32_t i = 1; i <= 100; ++i) {
        std::string file = logFile + "." + std::to_string(i);
        create_file(file);
        validFileList.emplace_back(file);
        utils::date_time::sleep_ms(2);  // 确保文件修改时间不同
    }

    const uint32_t startIdx = 200;
    const uint32_t step = 200;
    for (uint32_t i = startIdx; i <= RotatingFileSink::MAX_INDEX; i += step) {
        std::string file = logFile + "." + std::to_string(i);
        create_file(file);
        validFileList.emplace_back(file);
        utils::date_time::sleep_ms(5);  // 确保文件修改时间不同
    }

    // invalid file
    create_file(logFile);
    create_file(logFile + ".0");
    create_file(logFile + ".00");
    create_file(logFile + ".01");
    create_file(logFile + ".");
    create_file(logFile + ".a");
    create_file(logFile + "a");
    create_file(logFile + "." + std::to_string(RotatingFileSink::MAX_INDEX + 1));

    RotatingFileSink sink(logFile, false);

    auto fileList = sink.get_rotating_file_list();
    EXPECT_EQ(fileList.size(), validFileList.size());
    for (uint32_t i = 0; i < validFileList.size(); ++i) {
        EXPECT_EQ(fileList[i], validFileList[i]);
    }
}

TEST_F(TestRotatingFileSink, rotate1)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});
    TestRotate(logFile, {});
}

TEST_F(TestRotatingFileSink, rotate2)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});

    const uint32_t startIdx = 3;
    const uint32_t endIdx = 5;
    std::vector<uint32_t> existFileIdxs;

    for (uint32_t i = startIdx; i <= endIdx; ++i) {
        existFileIdxs.push_back(i);
    }

    TestRotate(logFile, existFileIdxs);
}

TEST_F(TestRotatingFileSink, rotate3)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});
    std::vector<uint32_t> existFileIdxs;
    for (uint32_t i = RotatingFileSink::MAX_INDEX - 3; i <= RotatingFileSink::MAX_INDEX; ++i) {
        existFileIdxs.push_back(i);
    }

    const uint32_t endIdx = 5;

    for (uint32_t i = RotatingFileSink::MIN_INDEX; i <= endIdx; ++i) {
        existFileIdxs.push_back(i);
    }

    TestRotate(logFile, existFileIdxs);
}

TEST_F(TestRotatingFileSink, rotate4)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});
    std::vector<uint32_t> existFileIdxs;
    for (uint32_t i = RotatingFileSink::MAX_INDEX - 3; i <= RotatingFileSink::MAX_INDEX; ++i) {
        existFileIdxs.push_back(i);
    }

    TestRotate(logFile, existFileIdxs);
}

TEST_F(TestRotatingFileSink, rotate_and_delete)
{
    std::string logFile = join_paths({_dir, get_logger_name(test_info_) + ".log"});

    uint32_t overflow_file_cnt = 10;
    uint32_t maxFileSize = 1024;
    uint32_t rotate_log_file = 2;

    for (uint32_t i = RotatingFileSink::MIN_INDEX; i <= overflow_file_cnt; ++i) {
        std::string file = logFile + "." + std::to_string(i);
        create_file(file);
        utils::date_time::sleep_ms(10);  // 确保文件修改时间不同
    }

    std::string nextLogFile = logFile + "." + std::to_string(overflow_file_cnt + rotate_log_file);

    RotatingFileSink sink(logFile, maxFileSize, 1, false);

    sink.set_pattern("%v");

    std::string logContent = std::string(100, 'a');

    size_t logMsgSize = logContent.size() + LF_LENGTH;
    size_t currSize = 0;
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
