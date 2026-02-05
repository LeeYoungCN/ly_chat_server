#include <cstddef>
#include <cstdint>
#include <string>

#include "common/common_error_code.h"
#include "common/constants/filesystem_constants.h"
#include "common/utils/file_writer.h"
#include "common/utils/filesystem_utils.h"
#include "common/utils/process_utils.h"
#include "gtest/gtest.h"

namespace test::test_utils::test_file_writer {
using namespace common::filesystem;
using namespace common::process;

class TestFileAppender : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override;
    void TearDown() override;

protected:
    std::string m_fileName{"test_file"};
    std::string m_suffix{".log"};
    std::string m_directory;
    std::string m_testFile;
    FileWriter m_appender;
};

void TestFileAppender::SetUp()
{
    m_directory = JoinPaths({GetProcessDirectory(), "file_writer"});
    m_testFile = JoinPaths({m_directory, m_fileName + m_suffix});
}

void TestFileAppender::TearDown()
{
    m_appender.close();
    DeleteDir(m_directory);
}

TEST_F(TestFileAppender, file_name_empty)
{
    m_appender.open("");
    EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_PARAM_EMPTY);
}

TEST_F(TestFileAppender, file_name_process)
{
    m_appender.open(m_testFile);
    EXPECT_EQ(m_appender.base_name(), m_fileName + m_suffix);
    EXPECT_EQ(m_appender.file_name(), m_fileName);
    EXPECT_EQ(m_appender.directory(), m_directory);
}

TEST_F(TestFileAppender, open_success)
{
    m_appender.open(m_testFile);
    EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_SUCCESS);
    EXPECT_TRUE(FileExists(m_testFile));
}

TEST_F(TestFileAppender, append_size)
{
    m_appender.open(m_testFile);
    EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_SUCCESS);
    EXPECT_EQ(m_appender.size(), 0);
    EXPECT_TRUE(FileExists(m_testFile));

    size_t expectSize = 0;
    for (uint32_t i = 0; i < 1024; i++) {
        std::string line = "line" + std::to_string(i);
        expectSize += line.length();
        m_appender.write(line);
        EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_SUCCESS);
        EXPECT_EQ(m_appender.size(), expectSize);
        EXPECT_GE(m_appender.size(), GetFileSize(m_testFile));
    }
    m_appender.flush();
    EXPECT_EQ(m_appender.size(), GetFileSize(m_testFile));
    m_appender.close();
    EXPECT_EQ(m_appender.size(), GetFileSize(m_testFile));
}

TEST_F(TestFileAppender, append_line_size)
{
    m_appender.open(m_testFile);
    EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_SUCCESS);
    EXPECT_EQ(m_appender.size(), 0);
    EXPECT_TRUE(FileExists(m_testFile));

    size_t expectSize = 0;
    for (uint32_t i = 0; i < 1024; i++) {
        std::string line = "line" + std::to_string(i);
        expectSize += line.length() + common::filesystem::LF_LENGTH;
        m_appender.write_line(line);
        EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_SUCCESS);
        EXPECT_EQ(m_appender.size(), expectSize);
        EXPECT_GE(m_appender.size(), GetFileSize(m_testFile));
    }
    m_appender.flush();
    EXPECT_EQ(m_appender.size(), GetFileSize(m_testFile));
    m_appender.close();
    EXPECT_EQ(m_appender.size(), GetFileSize(m_testFile));
}

TEST_F(TestFileAppender, append_mode)
{
    m_appender.open(m_testFile);
    EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_SUCCESS);
    EXPECT_EQ(m_appender.size(), 0);
    EXPECT_TRUE(FileExists(m_testFile));

    size_t expectSize = 0;
    for (uint32_t i = 0; i < 64; i++) {
        std::string line = "line" + std::to_string(i);
        expectSize += line.length() + common::filesystem::LF_LENGTH;
        m_appender.write_line(line);
        EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_SUCCESS);
        m_appender.close();
        m_appender.reopen(FileWriteMode::APPEND);
        EXPECT_EQ(m_appender.get_last_error(), ERR_COMM_SUCCESS);
        EXPECT_EQ(m_appender.size(), expectSize);
        EXPECT_GE(m_appender.size(), GetFileSize(m_testFile));
    }
    m_appender.flush();
    EXPECT_EQ(m_appender.size(), GetFileSize(m_testFile));
    m_appender.close();
    EXPECT_EQ(m_appender.size(), GetFileSize(m_testFile));
}

}  // namespace test::test_utils::test_file_writer
