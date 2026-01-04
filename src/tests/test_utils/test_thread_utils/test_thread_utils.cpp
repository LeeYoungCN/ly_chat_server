/**
 * @file test_thread_utils.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-08-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "common/utils/thread_utils.h"
#include "gtest/gtest.h"

TEST(TestThreadUtils, ThreadId)
{
    EXPECT_NE(get_curr_thread_id(), 0);
}

TEST(TestThreadUtils, ThreadName)
{
    EXPECT_STREQ(GetCurrThreadName(), "");
    SetCurrThreadName("main");
    EXPECT_STREQ(GetCurrThreadName(), "main");
}
