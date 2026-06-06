/**
 * @file test_filesystem_utils_file.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-30
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "common/compiler/macros.h"
#include "common/constants/filesystem_constants.h"
#include "common/types/filesystem_types.h"
#include "gtest/gtest.h"
#include "test_filesystem_utils_base.h"
#include "utils/filesystem_utils.h"
#include "utils/thread_utils.h"
#include "utils/utils_error_code.h"

namespace test::test_utils::test_filesystem_utils {

using namespace utils::filesystem;
using namespace constants::filesystem;

class TestFilesystemUtilsFile : public TestFilesystemUtilsBase {
protected:
    void SetUp() override;
    void TearDown() override;

protected:
    PathString m_testFile;
    PathString m_testFileName{"test_file"};
};

void TestFilesystemUtilsFile::SetUp()
{
    TestFilesystemUtilsBase::SetUp();
    m_testFile = to_absolute_path(m_testFileName, m_processDir);
}

void TestFilesystemUtilsFile::TearDown()
{
    TestFilesystemUtilsBase::TearDown();
    EXPECT_TRUE(delete_file(m_testFile));
}

TEST_F(TestFilesystemUtilsFile, file_exists)
{
    EXPECT_TRUE(file_exists(m_process));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, FileExists_Nonexistent)
{
    EXPECT_FALSE(file_exists(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, FileExists_TypeInvalid)
{
    EXPECT_FALSE(file_exists(m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_AlreadyExists)
{
    EXPECT_TRUE(utils::filesystem::create_file(m_process));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_ALREADY_EXISTS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_TypeInvalid)
{
    EXPECT_FALSE(utils::filesystem::create_file(m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_Success)
{
    EXPECT_TRUE(utils::filesystem::create_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_DirNotExist)
{
    auto newFile = to_absolute_path(m_testFileName, m_processDir + "/nonexistent");
    EXPECT_FALSE(utils::filesystem::create_file(newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, DeleteFile_DirNotExist)
{
    auto newFile = to_absolute_path(m_testFileName, m_processDir + "/nonexistent");
    EXPECT_TRUE(delete_file(newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, DeleteFile_TypeInvalid)
{
    EXPECT_FALSE(delete_file(m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, DeleteFile_FileExist)
{
    EXPECT_TRUE(utils::filesystem::create_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_TRUE(delete_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_TRUE(delete_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, ReadeFile_Success)
{
    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(read_text_file(m_testFile), "");
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(get_file_size(m_testFile), 0);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, ReadeFile_TargetInvalid)
{
    EXPECT_EQ(read_text_file(m_processDir), "");
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, ReadeFile_Nonexistent)
{
    EXPECT_EQ(read_text_file(m_testFile), "");
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_success)
{
    std::string text = "1234";
    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_TRUE(write_text_file(m_testFile, text));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(read_text_file(m_testFile), text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(get_file_size(m_testFile), text.size());
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_OverwriteSuccess)
{
    std::string text = "1234";
    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_TRUE(write_text_file(m_testFile, text, true));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(read_text_file(m_testFile), text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(get_file_size(m_testFile), text.size());
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_TRUE(write_text_file(m_testFile, text, true));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(read_text_file(m_testFile), text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(get_file_size(m_testFile), text.size());
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_AppendSuccess)
{
    std::string text = "1234\n";
    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_TRUE(write_text_file(m_testFile, text, false));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(read_text_file(m_testFile), text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
#if OS_WINDOWS
    EXPECT_EQ(get_file_size(m_testFile), text.length() + 1);
#else
    EXPECT_EQ(get_file_size(m_testFile), text.length());
#endif
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_TRUE(write_text_file(m_testFile, text, false));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(read_text_file(m_testFile), text + text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
#if OS_WINDOWS
    EXPECT_EQ(get_file_size(m_testFile), 2 * (text.length() + 1));
#else
    EXPECT_EQ(get_file_size(m_testFile), 2 * (text.length()));
#endif
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_Nonexistent)
{
    EXPECT_FALSE(write_text_file(m_testFile, ""));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FOUND);
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_TargetInvalid)
{
    EXPECT_FALSE(write_text_file(m_processDir, ""));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE);
}

TEST_F(TestFilesystemUtilsFile, CopyFile_Success)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";

    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_TRUE(write_text_file(m_testFile, text));
    EXPECT_TRUE(copy_file(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_EQ(read_text_file(m_testFile), read_text_file(newFile));
    EXPECT_EQ(get_file_size(m_testFile), get_file_size(newFile));

    EXPECT_TRUE(delete_file(newFile));
}

TEST_F(TestFilesystemUtilsFile, CopyFile_OverWriteSuccess)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_TRUE(copy_file(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_EQ(read_text_file(newFile), "");
    EXPECT_EQ(get_file_size(newFile), 0);

    std::string text = "12345";
    EXPECT_TRUE(write_text_file(m_testFile, text));
    EXPECT_TRUE(copy_file(m_testFile, newFile, true));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(read_text_file(m_testFile), read_text_file(newFile));
    EXPECT_EQ(get_file_size(m_testFile), get_file_size(newFile));
    EXPECT_TRUE(delete_file(newFile));
}

TEST_F(TestFilesystemUtilsFile, CopyFile_SrcNotExist)
{
    std::string newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_FALSE(copy_file(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_SrcInvalid)
{
    std::string newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_FALSE(copy_file(m_processDir, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_DstInvalid)
{
    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_FALSE(copy_file(m_testFile, m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_DstAlreadyExist)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_TRUE(copy_file(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_EQ(read_text_file(newFile), "");
    EXPECT_EQ(get_file_size(newFile), 0);
    std::string text = "1234";
    EXPECT_TRUE(write_text_file(m_testFile, text));
    EXPECT_FALSE(copy_file(m_testFile, newFile, false));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_ALREADY_EXISTS) << get_thread_last_err_msg();
    EXPECT_NE(read_text_file(m_testFile), read_text_file(newFile));
    EXPECT_NE(get_file_size(m_testFile), get_file_size(newFile));
    EXPECT_TRUE(delete_file(newFile));
}

TEST_F(TestFilesystemUtilsFile, RenameFile_Success)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";
    EXPECT_TRUE(delete_file(newFile));

    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_TRUE(write_text_file(m_testFile, text));

    EXPECT_TRUE(rename_file(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_FALSE(file_exists(m_testFile));
    EXPECT_TRUE(file_exists(newFile));

    EXPECT_EQ(read_text_file(newFile), text);
    EXPECT_EQ(get_file_size(newFile), text.size());

    EXPECT_TRUE(delete_file(newFile));
}

TEST_F(TestFilesystemUtilsFile, RenameFile_DestAlreadyExist)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";
    EXPECT_TRUE(delete_file(newFile));

    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_TRUE(create_file(newFile));

    EXPECT_TRUE(file_exists(m_testFile));
    EXPECT_TRUE(file_exists(newFile));

    EXPECT_TRUE(write_text_file(m_testFile, text));

    EXPECT_FALSE(rename_file(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_ALREADY_EXISTS) << get_thread_last_err_msg();

    EXPECT_TRUE(file_exists(m_testFile));

    EXPECT_EQ(read_text_file(newFile), "");
    EXPECT_EQ(get_file_size(newFile), 0);

    EXPECT_TRUE(delete_file(newFile));
}

TEST_F(TestFilesystemUtilsFile, RenameFile_Overwrite)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";
    EXPECT_TRUE(delete_file(newFile));

    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_TRUE(create_file(newFile));

    EXPECT_TRUE(file_exists(m_testFile));
    EXPECT_TRUE(file_exists(newFile));

    EXPECT_TRUE(write_text_file(m_testFile, text));

    EXPECT_TRUE(rename_file(m_testFile, newFile, true));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_FALSE(file_exists(m_testFile));
    EXPECT_TRUE(file_exists(newFile));

    EXPECT_EQ(read_text_file(newFile), text);
    EXPECT_EQ(get_file_size(newFile), text.size());

    EXPECT_TRUE(delete_file(newFile));
}

TEST_F(TestFilesystemUtilsFile, RenameFile_SrcNonexistent)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";

    EXPECT_FALSE(rename_file(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, RenameFile_SrcInvalid)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";

    EXPECT_FALSE(rename_file(m_processDir, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, RenameFile_DestInvalid)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";

    EXPECT_TRUE(create_file(m_testFile));
    EXPECT_TRUE(write_text_file(m_testFile, text));

    EXPECT_FALSE(rename_file(m_testFile, m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_UTILS_NOT_FILE) << get_thread_last_err_msg();
}

}  // namespace test::test_utils::test_filesystem_utils
