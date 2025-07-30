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
#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

#include "common/types/date_time_types.h"

namespace common::types::filesystem {
enum class PathType {
    Nonexistent,  // 不存在
    File,         // 是文件
    Directory     // 是文件夹
};

/**
 * @brief 路径字符串类型，用于表示文件系统路径
 */
using PathString = std::string;

/**
 * @brief 路径列表类型，用于存储多个路径片段或路径集合
 */
using PathList = std::vector<PathString>;

/**
 * @brief 文件大小类型，以字节为单位
 */
using FileSize = uint64_t;

/**
 * @brief 二进制数据容器，用于存储文件的二进制内容
 */
using ByteVector = std::vector<uint8_t>;

/**
 * @brief 文件/目录条目类型枚举
 */
enum class EntryType {
    NONEXISTENT = 0,   // 路径不存在
    FILE,              // 普通文件
    DIRECTORY,         // 目录
    SYMBOL_LINK,       // 符号链接
    CHARACTER_DEVICE,  // 字符设备
    BLOCK_DEVICE,      // 块设备
    FIFO,              // 管道(FIFO)
    SOCKET,            // 套接字
    UNKNOWN            // 未知类型
};
/**
 * @brief 文件权限枚举，跨平台统一封装
 */
enum class Permission {
    None = 0,                     ///< 无权限
    Read = 1 << 0,                ///< 读权限
    Write = 1 << 1,               ///< 写权限
    Execute = 1 << 2,             ///< 执行权限
    ReadWrite = Read | Write,     ///< 读写权限
    All = Read | Write | Execute  ///< 全部权限
};

/**
 * @brief 文件属性枚举
 */
enum class FileAttribute {
    None = 0,           ///< 无特殊属性
    Hidden = 1 << 0,    ///< 隐藏属性
    Readonly = 1 << 1,  ///< 只读属性
    System = 1 << 2     ///< 系统文件属性（仅Windows有效）
};

/**
 * @brief 文件信息结构体，包含文件的元数据
 */
struct FileInfo {
    PathString Path;                                 ///< 文件的完整路径
    FileSize Size;                                   ///< 文件大小（字节）
    EntryType Type;                                  ///< 条目类型（文件/目录等）
    common::types::date_time::TimeStamp CreateTime;  ///< 创建时间戳
    common::types::date_time::TimeStamp ModifyTime;  ///< 最后修改时间戳
    Permission Permissions;                          ///< 访问权限
    FileAttribute Attributes;                        ///< 文件属性（隐藏/只读等）
};

}  // namespace common::types::filesystem

#endif  // COMMON_TYPES_FILESYSTEM_TYPES_H
