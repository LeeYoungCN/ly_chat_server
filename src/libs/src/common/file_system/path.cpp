/**
 * @file path.cpp
 * @author LiYang
 * @brief
 * @version 0.1
 * @date 2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "common/file_system/path.h"

#include <filesystem>
#include <string>

#include "common/base/base_log.h"

namespace fs = std::filesystem;

namespace {
constexpr uint32_t MAX_PATH_LEN = 1024;
}

namespace proj {
std::string RelativeToAbsolutePath(const std::string& relPath, const std::string& basePath)
{
    try {
        fs::path base = basePath.empty() ? fs::current_path() : fs::path(basePath);
        fs::path relative(relPath);
        fs::path combined = base / relative;
        auto absPath = fs::absolute(combined).lexically_normal().string();
        return absPath;
    } catch (const fs::filesystem_error& e) {
        BASE_LOG_ERR("Failed to convert the path: %s", e.what());
        return "";
    }
}

std::string GetProcessDirectory()
{
    std::string process = GetProcessPath();
    fs::path proc(process);
    return proc.parent_path().string();
}


#ifdef _WIN32
#include <windows.h>
std::string GetProcessPath()
{
    return "";
}
#else
#include <unistd.h>
std::string GetProcessPath()
{
    char path[MAX_PATH_LEN] = {'\0'};
    auto length = readlink("/proc/self/exe", path, MAX_PATH_LEN - 1);
    if (length == -1) {
        BASE_LOG_ERR("Get process path fail");
        return "";
    }
    path[length] = '\0';
    return path;
}
#endif  // _WIN32

}  // namespace proj
