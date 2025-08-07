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

#include "common/types/thread_types.h"
#include "common/utils/thread_utils.h"
#include "gtest/gtest.h"

using namespace common::types::thread;
using namespace common::utils::thread;

TEST(TestThreadUtils, ThreadId)
{
    EXPECT_NE(GetCurrentThreadId(), 0);
}

TEST(TestThreadUtils, ThreadName)
{
    EXPECT_EQ(GetCurrentThreadName(), "");
    SetCurrentThreadName("main");
    EXPECT_EQ(GetCurrentThreadName(), "main");
}
