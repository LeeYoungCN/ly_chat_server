/**
 * @file path.h
 * @author LiYang
 * @brief 路径字符串处理, 仅处理字符串, 不判断路径是否合法
 * @version 0.1
 * @date 2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <string>

namespace proj {
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
}  // namespace proj
