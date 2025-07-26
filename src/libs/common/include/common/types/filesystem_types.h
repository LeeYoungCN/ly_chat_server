/**
 * @file filesystem_types.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-26
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once

#ifndef COMMON_TYPES_FILESYSTEM_TYPES_H
#define COMMON_TYPES_FILESYSTEM_TYPES_H

namespace common::types::fs {
enum class PathType {
    Nonexistent,  // 不存在
    File,         // 是文件
    Directory     // 是文件夹
};
}  // namespace common::types::fs

#endif  // COMMON_TYPES_FILESYSTEM_TYPES_H
