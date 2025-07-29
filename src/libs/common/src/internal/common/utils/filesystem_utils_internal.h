/**
 * @file filesystem_utils_internal.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef COMMON_UTILS_FILESYSTEM_UTILS_INTERNAL_H
#define COMMON_UTILS_FILESYSTEM_UTILS_INTERNAL_H

#include "common/constants/filesystem_constants.h"

namespace common::utils::filesystem::internal {
// 全局错误码（线程局部存储，避免多线程冲突）
static thread_local constants::filesystem::ErrorCode g_fileSystemLastError = constants::filesystem::ErrorCode::SUCCESS;

// 辅助函数：设置最后错误码
inline void SetLastError(constants::filesystem::ErrorCode code)
{
    g_fileSystemLastError = code;
}
}  // namespace common::utils::filesystem::internal

#endif  // COMMON_UTILS_FILESYSTEM_UTILS_INTERNAL_H
