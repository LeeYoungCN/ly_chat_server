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
#include "common/constants/filesystem_constants.h"
#include "common/types/filesystem_types.h"
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
    EXPECT_TRUE(DeleteFileUtils(m_testFile));
}

TEST_F(TestFilesystemUtilsFile, FileExists)
{
    EXPECT_TRUE(FileExists(m_process));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, FileExists_Nonexists)
{
    EXPECT_FALSE(FileExists(m_testFile));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, FileExists_TypeInvalid)
{
    EXPECT_FALSE(FileExists(m_processDir));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FILE) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_AlreadyExists)
{
    EXPECT_TRUE(common::utils::filesystem::CreateFileUtils(m_process));
    EXPECT_EQ(GetLastError(), ErrorCode::ALREADY_EXISTS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_TypeInvalid)
{
    EXPECT_FALSE(common::utils::filesystem::CreateFileUtils(m_processDir));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FILE) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_Success)
{
    EXPECT_TRUE(common::utils::filesystem::CreateFileUtils(m_testFile));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, CreateFile_DirNotExist)
{
    auto newFile = ToAbsolutePath(m_testFileName, m_processDir + "/nonexist");
    EXPECT_FALSE(common::utils::filesystem::CreateFileUtils(newFile));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, DelteteFile_DirNotExist)
{
    auto newFile = ToAbsolutePath(m_testFileName, m_processDir + "/nonexist");
    EXPECT_TRUE(DeleteFileUtils(newFile));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, DelteteFile_TypeInvalid)
{
    EXPECT_FALSE(DeleteFileUtils(m_processDir));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FILE) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, DelteteFile_FileExist)
{
    EXPECT_TRUE(common::utils::filesystem::CreateFileUtils(m_testFile));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
    EXPECT_TRUE(DeleteFileUtils(m_testFile));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
    EXPECT_TRUE(DeleteFileUtils(m_testFile));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_Success)
{
    EXPECT_TRUE(CreateFileUtils(m_testFile));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
    std::string newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_TRUE(CopyFileUtils(m_testFile, newFile));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();

    EXPECT_TRUE(FileExists(newFile));

    EXPECT_TRUE(CopyFileUtils(m_testFile, newFile, true));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
    EXPECT_TRUE(DeleteFileUtils(newFile));
}

TEST_F(TestFilesystemUtilsFile, CopyFile_SrcNotExist)
{
    std::string newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_FALSE(CopyFileUtils(m_testFile, newFile));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_SrcInvalid)
{
    std::string newFile = m_processDir + PATH_SEP + "file2";
    EXPECT_FALSE(CopyFileUtils(m_processDir, newFile));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FILE) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsFile, CopyFile_DstInvalid)
{
    EXPECT_TRUE(CreateFileUtils(m_testFile));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
    EXPECT_FALSE(CopyFileUtils(m_testFile, m_processDir));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FILE) << GetLastErrorString();
}

}  // namespace test::test_utils::test_filesystem_utils
