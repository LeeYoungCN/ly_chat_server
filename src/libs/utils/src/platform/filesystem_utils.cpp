#include "utils/filesystem_utils.h"

#include <unistd.h>

#include <filesystem>
#include <string>

#include "common/debug/assert.h"
#include "common/debug/log.h"
#include "utils/filesystem_utils.h"

namespace utils {
namespace fs = std::filesystem;

PathType CheckPathType(const std::string& path)
{
    try {
        if (!std::filesystem::exists(path)) {
            return PathType::Nonexistent;
        }
        return std::filesystem::is_directory(path) ? PathType::Directory : PathType::File;
    } catch (const std::filesystem::filesystem_error& e) {
        COMMON_ASSERT_MSG(false, "Check path type fail: {}", e.what());
        return PathType::Nonexistent;
    }
}

bool CreateDirectoryAndRemoveOld(const std::string& path)
{
    if (RemoveDirectory(path)) {
        return false;
    }
    return fs::create_directories(path);
}

bool CreateDirectoryIfNotExist(const std::string& path)
{
    switch (CheckPathType(path)) {
        case PathType::Directory:
            return true;
            break;
        case PathType::Nonexistent:
            try {
                return fs::create_directories(path);
            } catch (const fs::filesystem_error& e) {
                COMMON_ASSERT_MSG(false, "Failed to create directory: {}", e.what());
                return false;
            }
        default:
            return false;
            break;
    }
}

bool RemoveDirectory(const std::string& path)
{
    switch (CheckPathType(path)) {
        case PathType::Directory:
            try {
                auto n = fs::remove_all(path);
                COMMON_LOG_COND((n > 0), "Remove directory {}. n = {}", path, n);
                return n > 0;
            } catch (const fs::filesystem_error& e) {
                COMMON_LOG_INFO("Failed to remove direcotry: {}", e.what());
                return false;
            }
            break;
        case PathType::File:
            COMMON_LOG_FATAL("Failed to remove directory. {} is file.", path);
            return false;
            break;
        default:
            COMMON_LOG_INFO("Directory not exist. [{}]", path);
            return true;
            break;
    }
}

bool IsRegularFile(const std::string& file)
{
    return fs::exists(file) && fs::is_regular_file(file);
}
std::string RelativeToAbsolutePath(const std::string& relPath, const std::string& basePath)
{
    try {
        fs::path base = basePath.empty() ? fs::current_path() : fs::path(basePath);
        fs::path relative(relPath);
        fs::path combined = base / relative;
        auto absPath = fs::absolute(combined).lexically_normal().string();
        return absPath;
    } catch (const fs::filesystem_error& e) {
        COMMON_LOG_ERR("Failed to convert the path {}", e.what());
        return "";
    }
}

std::string GetProcessDirectory()
{
    std::string process = GetProcessPath();
    fs::path proc(process);
    return proc.parent_path().string();
}

}  // namespace utils
