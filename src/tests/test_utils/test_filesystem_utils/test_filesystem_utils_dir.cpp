#include "common/compiler/macros.h"
#include "common/constants/filesystem_constants.h"
#include "common/types/filesystem_types.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"
#include "test_filesystem_utils_base.h"

namespace test::test_utils::test_filesystem_utils {

using namespace common::utils::filesystem;
using namespace common::types::filesystem;
using namespace common::constants::filesystem;

class TestFilesystemUtilsDir : public TestFilesystemUtilsBase {
protected:
    void SetUp() override;
    void TearDown() override;

protected:
    PathString m_testDir1;
    PathString m_testDir2;
};

void TestFilesystemUtilsDir::SetUp()
{
    TestFilesystemUtilsBase::SetUp();

    m_testDir1 = ToAbsolutePath("./test_dir1", m_processDir);
    m_testDir2 = ToAbsolutePath("./test_dir2", m_testDir1);
    EXPECT_TRUE(DeleteDir(m_testDir1, true));
    EXPECT_FALSE(DirExists(m_testDir1));
}

void TestFilesystemUtilsDir::TearDown()
{
    EXPECT_TRUE(DeleteDir(m_testDir1, true));
}

TEST_F(TestFilesystemUtilsDir, DirExists_Exist)
{
    EXPECT_TRUE(DirExists(m_processDir));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
}

TEST_F(TestFilesystemUtilsDir, DirExists_NonExist)
{
    EXPECT_FALSE(DirExists(m_testDir1));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND);
}

TEST_F(TestFilesystemUtilsDir, DirExists_TargetInvalid)
{
    EXPECT_FALSE(DirExists(m_process));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_DIRECTORY);
}

TEST_F(TestFilesystemUtilsDir, DeleteDir_NotExist)
{
    EXPECT_TRUE(DeleteDir(m_testDir1));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND);
    EXPECT_TRUE(DeleteDir(m_process + "/nonexistent"));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND);
}

TEST_F(TestFilesystemUtilsDir, DeleteDir_TypeInvalid)
{
    EXPECT_FALSE(DeleteDir(m_process));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_DIRECTORY);
}

TEST_F(TestFilesystemUtilsDir, CreateDir_TypeInvalid)
{
    EXPECT_FALSE(CreateDir(m_process));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_DIRECTORY) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsDir, CreateDir_AlreadyExist)
{
    EXPECT_TRUE(CreateDir(m_processDir));
    EXPECT_EQ(GetLastError(), ErrorCode::ALREADY_EXISTS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsDir, CreateDir_NotRecursiveSuccess)
{
    EXPECT_TRUE(CreateDir(m_testDir1, false));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
    EXPECT_FALSE(DirExists(m_testDir2));
    EXPECT_TRUE(CreateDir(m_testDir2, false));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
    EXPECT_TRUE(DirExists(m_testDir2));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsDir, CreateDir_NotRecursiveFalse)
{
    EXPECT_FALSE(CreateDir(m_testDir2, false));
    EXPECT_EQ(GetLastError(), ErrorCode::NOT_FOUND) << GetLastErrorString();
    EXPECT_FALSE(DirExists(m_testDir2)) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsDir, CreateDir_RecursiveSuccess)
{
    EXPECT_FALSE(DirExists(m_testDir1));
    EXPECT_TRUE(CreateDir(m_testDir2, true));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
    EXPECT_TRUE(DirExists(m_testDir2));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsDir, DeleteDir_NotRecursiveFalse)
{
    EXPECT_TRUE(CreateDir(m_testDir2, true));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_TRUE(DirExists(m_testDir2));

    EXPECT_FALSE(DeleteDir(m_testDir1, false));
#if COMPILER_MINGW
    EXPECT_EQ(GetLastError(), ErrorCode::IO_ERROR) << GetLastErrorString();
#else
    EXPECT_EQ(GetLastError(), ErrorCode::DIR_NOT_EMPTY) << GetLastErrorString();
#endif
    EXPECT_TRUE(DirExists(m_testDir1));
}

TEST_F(TestFilesystemUtilsDir, DeleteDir_NotRecursiveSuccess)
{
    EXPECT_TRUE(CreateDir(m_testDir2, true));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_TRUE(DirExists(m_testDir2));

    EXPECT_TRUE(DeleteDir(m_testDir2, false));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_FALSE(DirExists(m_testDir2));

    EXPECT_TRUE(DeleteDir(m_testDir1, false));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_FALSE(DirExists(m_testDir1));
}

TEST_F(TestFilesystemUtilsDir, DeleteDir_RecursiveSuccess)
{
    EXPECT_TRUE(CreateDir(m_testDir2, true));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_TRUE(DirExists(m_testDir2));

    EXPECT_TRUE(DeleteDir(m_testDir2, true));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_FALSE(DirExists(m_testDir2));
}

}  // namespace test::test_utils::test_filesystem_utils
