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
#ifndef FILESYSTEM_UTILS_H
#define FILESYSTEM_UTILS_H

#include <string>

namespace utils {
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

/**
 * @brief 相对路径转绝对路径。
 *
 * @param relPath 相对路径
 * @return std::string
 */
std::string RelativeToAbsolutePath(const std::string& relPath, const std::string& basePath = "");
/**
 * @brief Get the Process Path object
 *
 * @return std::string
 */
std::string GetProcessPath();

std::string GetProcessDirectory();
/**
 * @brief 获取当前工作路径。
 *
 * @return std::string
 */
std::string GetWorkingPath();
}  // namespace utils
#endif  // FILESYSTEM_UTILS_H
