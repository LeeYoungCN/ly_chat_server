/**
 * @file thread_utils.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-08-07
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#ifndef COMMON_UTILS_THREAD_UTILS_H
#define COMMON_UTILS_THREAD_UTILS_H

#include "common/types/thread_types.h"

namespace common::utils::thread {

/**
 * @brief 获取当前内核态线程ID
 *
 * @return 当前线程ID
 */
common::types::thread::ThreadId GetCurrentThreadId();

/**
 * @brief 设置线程名称
 *
 * @param name 线程名称
 */
void SetCurrentThreadName(const common::types::thread::ThreadName &name);

/**
 * @brief 获取当前线程名称
 *
 * @return 当前线程名称，未设置则为空
 */
common::types::thread::ThreadName GetCurrentThreadName();

}  // namespace common::utils::thread

#endif  // COMMON_UTILS_THREAD_UTILS_H
