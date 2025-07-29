/**
 * @file filesystem_constants.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-27
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef COMMON_CONSTANTS_FILESYSTEM_CONSTANTS_H
#define COMMON_CONSTANTS_FILESYSTEM_CONSTANTS_H

#include <cstddef>

namespace common::constants::filesystem {
/**
 * @brief 路径长度常量（跨平台自动适配）
 */
#ifdef _WIN32
constexpr size_t MAX_PATH_STD = 260;     ///< Windows标准路径最大长度（含终止符）
constexpr size_t MAX_PATH_LONG = 32767;  ///< Windows长路径最大长度（启用后）
#else
constexpr size_t MAX_PATH_STD = 4096;    ///< Unix系统标准路径最大长度
constexpr size_t MAX_PATH_LONG = 65536;  ///< Unix系统扩展路径长度
#endif

/**
 * @brief 文件名最大长度（不含路径部分）
 */
#ifdef _WIN32
constexpr size_t MAX_FILENAME = 256;  ///< Windows文件名长度限制
#else
constexpr size_t MAX_FILENAME = 255;  ///< Unix文件名长度限制
#endif

/**
 * @brief 路径分隔符（平台相关）
 */
#ifdef _WIN32
constexpr const char* PATH_SEP = "\\";     ///< Windows系统路径分隔符
constexpr const char* ALT_PATH_SEP = "/";  ///< Windows兼容的替代分隔符
#else
constexpr const char* PATH_SEP = "/";     ///< Unix系统路径分隔符
constexpr const char* ALT_PATH_SEP = "";  ///< 无替代分隔符
#endif

/**
 * @brief 错误码枚举，跨平台统一的错误标识
 */
enum class ErrorCode {
    SUCCESS = 0,            ///< 操作成功
    NOT_FOUND = 1,          ///< 路径不存在
    PERMISSION_DENIED = 2,  ///< 权限不足
    PATH_TOO_LONG = 3,      ///< 路径长度超过系统限制
    ALREADY_EXISTS = 4,     ///< 目标路径已存在
    NOT_DIRECTORY = 5,      ///< 路径指向的不是目录
    NOT_FILE = 6,           ///< 路径指向的不是文件
    IO_ERROR = 7,           ///< I/O操作错误
    SYSTEM_ERROR = 100      ///< 系统级错误
};

}  // namespace common::constants::filesystem

#endif  // COMMON_CONSTANTS_FILESYSTEM_CONSTANTS_H
