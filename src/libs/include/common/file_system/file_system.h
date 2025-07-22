/**
 * @file file_system.h
 * @author LiYang
 * @brief 文件系统封装
 * @version 0.1
 * @date 2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef PROJ_FILE_SYSTEM_H
#define PROJ_FILE_SYSTEM_H

#include <string>

namespace proj {
enum class PathType {
    Nonexistent,  // 不存在
    File,         // 是文件
    Directory     // 是文件夹
};

/**
 * @brief 创建文件夹，如果存在，则删除。
 *
 * @param path 文件夹路径
 * @return true
 * @return false
 */
bool CreateDirectoryAndRemoveOld(const std::string& path);

/**
 * @brief 创建文件夹，如果文件夹存在,则不再创建。
 *
 * @param path 文件夹路径
 * @return true
 * @return false
 */
bool CreateDirectoryIfNotExist(const std::string& path);

/**
 * @brief 删除文件夹，并递归删除文件夹内的所有文件。
 *
 * @param path 文件夹路径
 * @return true
 * @return false
 */
bool RemoveDirectory(const std::string& path);

/**
 * @brief 判断文件是否存在。
 *
 * @param file 文件路径
 * @return true
 * @return false
 */
bool IsRegularFile(const std::string& file);
}  // namespace proj
#endif  // PROJ_FILE_SYSTEM_H
