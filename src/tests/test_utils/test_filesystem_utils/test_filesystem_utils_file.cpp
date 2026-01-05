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
#include "common/common_error_code.h"
#include "common/compiler/macros.h"
#include "common/constants/filesystem_constants.h"
#include "common/types/filesystem_types.h"
#include "common/utils/error_code_utils.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"
#include "test_filesystem_utils_base.h"

namespace test::test_utils::test_filesystem_utils {

using namespace common::utils::filesystem;
using namespace common::types::filesystem;
using namespace common::constants::filesystem;

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
    m_testFile = ToAbsolutePath(m_testFileName, m_processDir);
}

void TestFilesystemUtilsFile::TearDown()
{
    TestFilesystemUtilsBase::TearDown();
    EXPECT_TRUE(DeleteFile(m_testFile));
}

TEST_F(TestFilesystemUtilsFile, FileExists)
{
    EXPECT_TRUE(FileExists(m_process));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, FileExists_Nonexistent)
{
    EXPECT_FALSE(FileExists(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, FileExists_TypeInvalid)
{
    EXPECT_FALSE(FileExists(m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_AlreadyExists)
{
    EXPECT_TRUE(common::utils::filesystem::CreateFile(m_process));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_ALREADY_EXISTS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_TypeInvalid)
{
    EXPECT_FALSE(common::utils::filesystem::CreateFile(m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_Success)
{
    EXPECT_TRUE(common::utils::filesystem::CreateFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_DirNotExist)
{
    auto newFile = ToAbsolutePath(m_testFileName, m_processDir + "/nonexistent");
    EXPECT_FALSE(common::utils::filesystem::CreateFile(newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, DeleteFile_DirNotExist)
{
    auto newFile = ToAbsolutePath(m_testFileName, m_processDir + "/nonexistent");
    EXPECT_TRUE(DeleteFile(newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, DeleteFile_TypeInvalid)
{
    EXPECT_FALSE(DeleteFile(m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, DeleteFile_FileExist)
{
    EXPECT_TRUE(common::utils::filesystem::CreateFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_TRUE(DeleteFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_TRUE(DeleteFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, ReadeFile_Success)
{
    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(ReadTextFile(m_testFile), "");
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(GetFileSize(m_testFile), 0);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, ReadeFile_TargetInvalid)
{
    EXPECT_EQ(ReadTextFile(m_processDir), "");
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, ReadeFile_Nonexistent)
{
    EXPECT_EQ(ReadTextFile(m_testFile), "");
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_success)
{
    std::string text = "1234";
    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_TRUE(WriteTextFile(m_testFile, text));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(ReadTextFile(m_testFile), text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(GetFileSize(m_testFile), text.size());
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_OverwriteSuccess)
{
    std::string text = "1234";
    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_TRUE(WriteTextFile(m_testFile, text, true));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(ReadTextFile(m_testFile), text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(GetFileSize(m_testFile), text.size());
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_TRUE(WriteTextFile(m_testFile, text, true));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(ReadTextFile(m_testFile), text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(GetFileSize(m_testFile), text.size());
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_AppendSuccess)
{
    std::string text = "1234\n";
    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_TRUE(WriteTextFile(m_testFile, text, false));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(ReadTextFile(m_testFile), text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
#if PLATFORM_WINDOWS
    EXPECT_EQ(GetFileSize(m_testFile), text.length() + 1);
#else
    EXPECT_EQ(GetFileSize(m_testFile), text.length());
#endif
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_TRUE(WriteTextFile(m_testFile, text, false));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(ReadTextFile(m_testFile), text + text);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
#if PLATFORM_WINDOWS
    EXPECT_EQ(GetFileSize(m_testFile), 2 * (text.length() + 1));
#else
    EXPECT_EQ(GetFileSize(m_testFile), 2 * (text.length()));
#endif
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_Nonexistent)
{
    EXPECT_FALSE(WriteTextFile(m_testFile, ""));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FOUND);
}

TEST_F(TestFilesystemUtilsFile, WriteTextFile_TargetInvalid)
{
    EXPECT_FALSE(WriteTextFile(m_processDir, ""));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE);
}

TEST_F(TestFilesystemUtilsFile, CopyFile_Success)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";

    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_TRUE(WriteTextFile(m_testFile, text));
    EXPECT_TRUE(CopyFile(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_EQ(ReadTextFile(m_testFile), ReadTextFile(newFile));
    EXPECT_EQ(GetFileSize(m_testFile), GetFileSize(newFile));

    EXPECT_TRUE(DeleteFile(newFile));
}

TEST_F(TestFilesystemUtilsFile, CopyFile_OverWriteSuccess)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_TRUE(CopyFile(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_EQ(ReadTextFile(newFile), "");
    EXPECT_EQ(GetFileSize(newFile), 0);

    std::string text = "12345";
    EXPECT_TRUE(WriteTextFile(m_testFile, text));
    EXPECT_TRUE(CopyFile(m_testFile, newFile, true));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_EQ(ReadTextFile(m_testFile), ReadTextFile(newFile));
    EXPECT_EQ(GetFileSize(m_testFile), GetFileSize(newFile));
    EXPECT_TRUE(DeleteFile(newFile));
}

TEST_F(TestFilesystemUtilsFile, CopyFile_SrcNotExist)
{
    std::string newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_FALSE(CopyFile(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_SrcInvalid)
{
    std::string newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_FALSE(CopyFile(m_processDir, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_DstInvalid)
{
    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
    EXPECT_FALSE(CopyFile(m_testFile, m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_DstAlreadyExist)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_TRUE(CopyFile(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_EQ(ReadTextFile(newFile), "");
    EXPECT_EQ(GetFileSize(newFile), 0);
    std::string text = "1234";
    EXPECT_TRUE(WriteTextFile(m_testFile, text));
    EXPECT_FALSE(CopyFile(m_testFile, newFile, false));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_ALREADY_EXISTS) << get_thread_last_err_msg();
    EXPECT_NE(ReadTextFile(m_testFile), ReadTextFile(newFile));
    EXPECT_NE(GetFileSize(m_testFile), GetFileSize(newFile));
    EXPECT_TRUE(DeleteFile(newFile));
}

TEST_F(TestFilesystemUtilsFile, RenameFile_Success)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";
    EXPECT_TRUE(DeleteFile(newFile));

    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_TRUE(WriteTextFile(m_testFile, text));

    EXPECT_TRUE(RenameFile(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_FALSE(FileExists(m_testFile));
    EXPECT_TRUE(FileExists(newFile));

    EXPECT_EQ(ReadTextFile(newFile), text);
    EXPECT_EQ(GetFileSize(newFile), text.size());

    EXPECT_TRUE(DeleteFile(newFile));
}

TEST_F(TestFilesystemUtilsFile, RenameFile_DestAlreadyExist)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";
    EXPECT_TRUE(DeleteFile(newFile));

    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_TRUE(CreateFile(newFile));

    EXPECT_TRUE(FileExists(m_testFile));
    EXPECT_TRUE(FileExists(newFile));

    EXPECT_TRUE(WriteTextFile(m_testFile, text));

    EXPECT_FALSE(RenameFile(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_ALREADY_EXISTS) << get_thread_last_err_msg();

    EXPECT_TRUE(FileExists(m_testFile));

    EXPECT_EQ(ReadTextFile(newFile), "");
    EXPECT_EQ(GetFileSize(newFile), 0);

    EXPECT_TRUE(DeleteFile(newFile));
}

TEST_F(TestFilesystemUtilsFile, RenameFile_Overwrite)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";
    EXPECT_TRUE(DeleteFile(newFile));

    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_TRUE(CreateFile(newFile));

    EXPECT_TRUE(FileExists(m_testFile));
    EXPECT_TRUE(FileExists(newFile));

    EXPECT_TRUE(WriteTextFile(m_testFile, text));

    EXPECT_TRUE(RenameFile(m_testFile, newFile, true));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();

    EXPECT_FALSE(FileExists(m_testFile));
    EXPECT_TRUE(FileExists(newFile));

    EXPECT_EQ(ReadTextFile(newFile), text);
    EXPECT_EQ(GetFileSize(newFile), text.size());

    EXPECT_TRUE(DeleteFile(newFile));
}

TEST_F(TestFilesystemUtilsFile, RenameFile_SrcNonexistent)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";

    EXPECT_FALSE(RenameFile(m_testFile, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FOUND) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, RenameFile_SrcInvalid)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";

    EXPECT_FALSE(RenameFile(m_processDir, newFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsFile, RenameFile_DestInvalid)
{
    PathString newFile = m_processDir + PATH_SEP + "file2";
    std::string text = "12345";

    EXPECT_TRUE(CreateFile(m_testFile));
    EXPECT_TRUE(WriteTextFile(m_testFile, text));

    EXPECT_FALSE(RenameFile(m_testFile, m_processDir));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_NOT_FILE) << get_thread_last_err_msg();
}

}  // namespace test::test_utils::test_filesystem_utils
