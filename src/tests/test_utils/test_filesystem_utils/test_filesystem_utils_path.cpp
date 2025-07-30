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

TEST_F(TestFilesystemUtilsPath, AbosolutePath)
{
    EXPECT_TRUE(IsAbsolutePath(m_process));
    EXPECT_TRUE(IsAbsolutePath(m_workDir));
    EXPECT_TRUE(IsAbsolutePath(m_processDir));
}

TEST_F(TestFilesystemUtilsPath, ToAbosolute)
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
    PathString input{"a/b/../c/./d"};
    auto result = NormalizePath(input);
    EXPECT_EQ(result, m_workDir + "/a/c/d");
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsPath, NormalizePath_EmptyPath)
{
    PathString result = NormalizePath("");

    EXPECT_EQ(result, m_workDir);
    EXPECT_EQ(GetLastError(), ErrorCode::SUCCESS) << GetLastErrorString();
}

TEST_F(TestFilesystemUtilsPath, PathTooLong)
{
    // 构造超长路径（Windows默认路径限制约260字符，Linux无严格限制）
    PathString long_path;
#ifdef _WIN32
    // Windows下构造超过MAX_PATH的路径
    long_path = "C:\\";
    while (long_path.size() < 300) {
        long_path += "very_long_directory_name/";
    }
    long_path += "file.txt";
#else
    // Linux下构造超长路径（实际可能不会触发，仅作示例）
    long_path = "/";
    while (long_path.size() < 4096) {  // Linux通常支持4096字符
        long_path += "a/";
    }
#endif

    PathString result = NormalizePath(long_path);

    EXPECT_TRUE(result.empty());
    EXPECT_EQ(GetLastError(), ErrorCode::PATH_TOO_LONG) << GetLastErrorString();
}
}  // namespace test::test_utils::test_filesystem_utils
