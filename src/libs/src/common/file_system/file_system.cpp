#include "common/file_system/file_system.h"

#include <filesystem>

#include "common/base/base_log.h"

namespace proj {
namespace fs = std::filesystem;

PathType CheckPathType(const std::string& path)
{
    try {
        if (!std::filesystem::exists(path)) {
            return PathType::Nonexistent;
        }
        return std::filesystem::is_directory(path) ? PathType::Directory : PathType::File;
    } catch (const std::filesystem::filesystem_error& e) {
        BASE_LOG_ERR("Check path type fail: %s", e.what());
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
                BASE_LOG_ERR("Failed to create directory: %s", e.what());
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
                return fs::remove_all(path) > 0;
            } catch (const fs::filesystem_error& e) {
                BASE_LOG_ERR("Remove folder fail: %s", e.what());
                return false;
            }
            break;
        case PathType::File:
            return false;
            break;
        default:
            return true;
            break;
    }
}

bool IsRegularFile(const std::string& file)
{
    return fs::exists(file) && fs::is_regular_file(file);
}
}  // namespace proj
