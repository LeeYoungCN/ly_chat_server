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

#include "common/types/error_code_types.h"
#include "common/types/filesystem_types.h"
#include "gtest/gtest.h"
#include "utils/filesystem_utils.h"
#include "utils/process_utils.h"
#include "utils/thread_utils.h"
#include "utils/utils_error_code.h"

namespace test::test_utils::test_filesystem_utils {

class TestFilesystemUtilsBase : public ::testing::Test {
protected:
    static void SetUpTestSuite() {}
    static void TearDownTestSuite() {}
    void SetUp() override
    {
        m_process = utils::process::get_proc_path();
        EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
        m_processDir = utils::filesystem::GetDirectory(m_process);
        EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
        m_workDir = utils::filesystem::GetCurrentWorkingDirectory();
        EXPECT_EQ(get_thread_last_err(), ERR_COMM_SUCCESS);
    };
    void TearDown() override {};

protected:
    utils::filesystem::PathString m_process;
    utils::filesystem::PathString m_processDir;
    utils::filesystem::PathString m_workDir;
};

}  // namespace test::test_utils::test_filesystem_utils
