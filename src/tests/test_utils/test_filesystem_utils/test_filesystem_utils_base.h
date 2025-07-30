/**
 * @file test_filesystem_utils_base.h
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

namespace test::test_utils::test_filesystem_utils {

class TestFilesystemUtilsBase : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override
    {
        m_process = common::utils::filesystem::GetProcessPath();
        EXPECT_EQ(common::utils::filesystem::GetLastError(), common::constants::filesystem::ErrorCode::SUCCESS);
        m_processDir = common::utils::filesystem::GetDirName(m_process);
        EXPECT_EQ(common::utils::filesystem::GetLastError(), common::constants::filesystem::ErrorCode::SUCCESS);
        m_workDir = common::utils::filesystem::GetCurrentWorkingDirectory();
        EXPECT_EQ(common::utils::filesystem::GetLastError(), common::constants::filesystem::ErrorCode::SUCCESS);
    };
    void TearDown() override {};

protected:
    common::types::filesystem::PathString m_process;
    common::types::filesystem::PathString m_processDir;
    common::types::filesystem::PathString m_workDir;
};

}  // namespace test::test_utils::test_filesystem_utils
