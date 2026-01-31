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
#include <cstddef>

#include "common/common_error_code.h"
#include "common/constants/filesystem_constants.h"
#include "common/types/filesystem_types.h"
#include "common/utils/error_code_utils.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"
#include "test_filesystem_utils_base.h"

namespace test::test_utils::test_filesystem_utils {

using namespace common::filesystem;

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
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
    EXPECT_TRUE(IsAbsolutePath(absFile));
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
    EXPECT_EQ(GetEntryType(m_process), EntryType::FILE);
}

TEST_F(TestFilesystemUtilsPath, BaseName)
{
    auto fileName = GetFileName(m_process);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
    auto extention = GetExtension(m_process);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
    PathString fileWithExt = fileName + extention;
    auto baseName = GetBaseName(m_process);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
    EXPECT_EQ(baseName, fileWithExt);
}

TEST_F(TestFilesystemUtilsPath, NormalizePath)
{
    auto baseName = GetBaseName(m_process);
    auto testPath = m_processDir + PATH_SEP + "." + PATH_SEP + baseName;
    auto rstPath = NormalizePath(testPath);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
    EXPECT_EQ(m_process, rstPath);
    EXPECT_TRUE(IsAbsolutePath(rstPath));
}

TEST_F(TestFilesystemUtilsPath, NormalizePath_WithDots)
{
    PathString test = "a/b/../c/./d";
    PathString expect(MAX_PATH_STD, '\0');
    auto len = snprintf(expect.data(), MAX_PATH_STD, "a%sc%sd", PATH_SEP, PATH_SEP);
    expect.resize(static_cast<size_t>(len));
    auto result = NormalizePath(test);
    EXPECT_EQ(result, expect);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsPath, NormalizePath_EmptyPath)
{
    PathString result = NormalizePath("");

    EXPECT_EQ(result, "");
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsPath, JoinPaths_empty)
{
    EXPECT_EQ(JoinPaths({}), "");
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_PATH_INVALID) << get_thread_last_err_msg();
}

TEST_F(TestFilesystemUtilsPath, JoinPaths_Success)
{
    PathList test = {"a", "b", "c"};
    PathString expect(MAX_PATH_STD, '\0');
    auto size = snprintf(expect.data(), MAX_PATH_STD, "a%sb%sc", PATH_SEP, PATH_SEP);
    expect.resize(static_cast<size_t>(size));
    EXPECT_EQ(JoinPaths(test), expect);
    EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS) << get_thread_last_err_msg();
}

}  // namespace test::test_utils::test_filesystem_utils
