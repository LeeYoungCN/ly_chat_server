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

#include "common/common_error_code.h"
#include "common/types/error_code_types.h"
#include "common/types/filesystem_types.h"
#include "common/utils/error_code_utils.h"
#include "common/utils/filesystem_utils.h"
#include "gtest/gtest.h"

namespace test::test_utils::test_filesystem_utils {

class TestFilesystemUtilsBase : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override
    {
        m_process = common::filesystem::GetProcessPath();
        EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
        m_processDir = common::filesystem::GetDirectory(m_process);
        EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
        m_workDir = common::filesystem::GetCurrentWorkingDirectory();
        EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
    };
    void TearDown() override {};

protected:
    common::filesystem::PathString m_process;
    common::filesystem::PathString m_processDir;
    common::filesystem::PathString m_workDir;
};

}  // namespace test::test_utils::test_filesystem_utils
