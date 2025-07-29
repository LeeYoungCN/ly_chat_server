#include <cmath>
#include <filesystem>

#include "common/constants/filesystem_constants.h"
#include "common/types/filesystem_types.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"

namespace test::test_utils::test_filesystem_utils {

using namespace common::utils::filesystem;
using namespace common::types::filesystem;
using namespace common::constants::filesystem;

class TestFilesystemUtils : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override;
    void TearDown() override;

protected:
    PathString m_process;
    PathString m_processDir;
    PathString m_workDir;
};

void TestFilesystemUtils::SetUp()
{
    m_process = GetProcessPath();
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    m_processDir = GetDirName(m_process);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    m_workDir = GetCurrentWorkingDirectory();
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
}

void TestFilesystemUtils::TearDown() {}

TEST_F(TestFilesystemUtils, ProcessPath)
{
    EXPECT_FALSE(m_process.empty());

    EXPECT_EQ(GetEtryType(m_process), EntryType::File);
}

TEST_F(TestFilesystemUtils, ProcessDir)
{

    EXPECT_FALSE(m_processDir.empty());
    EXPECT_EQ(GetEtryType(m_processDir), EntryType::Directory);
}

TEST_F(TestFilesystemUtils, WorkingDirectory)
{
    EXPECT_FALSE(m_workDir.empty());
    EXPECT_EQ(GetEtryType(m_workDir), EntryType::Directory);
}

TEST_F(TestFilesystemUtils, AbosolutePath)
{
    EXPECT_TRUE(IsAbsolutePath(m_process));
    EXPECT_TRUE(IsAbsolutePath(m_workDir));
    EXPECT_TRUE(IsAbsolutePath(m_processDir));
}

TEST_F(TestFilesystemUtils, ToAbosolute)
{
    auto fileName = GetFileName(m_process);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    auto extention = GetExtension(m_process);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    std::cout << "fileName: " << fileName << std::endl;
    std::cout << "extention: " << extention << std::endl;
}

}  // namespace test::test_utils::test_filesystem_utils
