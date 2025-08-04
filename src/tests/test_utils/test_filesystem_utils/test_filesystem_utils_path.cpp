/**
 * @file test_filesystem_utils.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-29
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

class TestFilesystemUtilsPath : public TestFilesystemUtilsBase {};

TEST_F(TestFilesystemUtilsPath, ProcessPath)
{
    EXPECT_FALSE(m_process.empty());
    EXPECT_EQ(GetEntryType(m_process), EntryType::FILE);
}

TEST_F(TestFilesystemUtilsPath, ProcessDir)
{
    EXPECT_FALSE(m_processDir.empty());
    EXPECT_EQ(GetEntryType(m_processDir), EntryType::DIRECTORY);
}

TEST_F(TestFilesystemUtilsPath, WorkingDirectory)
{
    EXPECT_FALSE(m_workDir.empty());
    EXPECT_EQ(GetEntryType(m_workDir), EntryType::DIRECTORY);
}

TEST_F(TestFilesystemUtilsPath, AbsolutePath)
{
    EXPECT_TRUE(IsAbsolutePath(m_process));
    EXPECT_TRUE(IsAbsolutePath(m_workDir));
    EXPECT_TRUE(IsAbsolutePath(m_processDir));
}

TEST_F(TestFilesystemUtilsPath, ToAbsolute)
{
    auto baseName = GetBaseName(m_process);
    PathString relPah = std::string(".") + PATH_SEP + baseName;
    EXPECT_FALSE(IsAbsolutePath(relPah));
    auto absFile = ToAbsolutePath(relPah, m_processDir);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_TRUE(IsAbsolutePath(absFile));
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_EQ(GetEntryType(m_process), EntryType::FILE);
}

TEST_F(TestFilesystemUtilsPath, BaseName)
{
    auto fileName = GetFileName(m_process);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    auto extention = GetExtension(m_process);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    PathString fileWithExt = fileName + extention;
    auto baseName = GetBaseName(m_process);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_EQ(baseName, fileWithExt);
}

TEST_F(TestFilesystemUtilsPath, NormalizePath)
{
    auto baseName = GetBaseName(m_process);
    auto testPath = m_processDir + PATH_SEP + "." + PATH_SEP + baseName;
    auto rstPath = NormalizePath(testPath);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS);
    EXPECT_EQ(m_process, rstPath);
    EXPECT_TRUE(IsAbsolutePath(rstPath));
}

TEST_F(TestFilesystemUtilsPath, NormalizePath_WithDots)
{
    PathString test = "a/b/../c/./d";
    PathString expect = "a" + std::string(PATH_SEP) + "c" + std::string(PATH_SEP) + "d";

    auto result = NormalizePath(test);
    EXPECT_EQ(result, expect);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsPath, NormalizePath_EmptyPath)
{
    PathString result = NormalizePath("");

    EXPECT_EQ(result, "");
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
}

}  // namespace test::test_utils::test_filesystem_utils
