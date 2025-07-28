#include "common/utils/filesystem_utils.h"

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>  // Linux的readlink函数
#elif defined(__APPLE__)
#include <mach-o/dyld.h>  // macOS的_NSGetExecutablePath
#endif
#include <filesystem>
#include <string>

#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_log.h"
#include "common/types/filesystem_types.h"
#include "common/utils/filesystem_utils.h"

namespace common::utils::filesystem {

namespace stdfs = std::filesystem;
using namespace common::constants::filesystem;

std::string GetProcessPath()
{
    char path[MAX_PATH_LEN] = {'\0'};
#ifdef _WIN32
    DWORD length = GetModuleFileNameA(nullptr, path, MAX_PATH_LEN);
    if (length == 0 || length >= MAX_PATH_LEN) {
        COMMON_LOG_ERR("[WIN32]Failed to get process path, length: {}", length);
        length = 0;
    }
#elif defined(__linux__)
    auto length = readlink("/proc/self/exe", path, MAX_PATH_LEN - 1);
    if (length == -1) {
        COMMON_LOG_ERR("[Linux]Failed to get process path.");
        length = 0;
    }
    path[length] = '\0';
#elif defined(__APPLE__)
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        COMMON_LOG_ERR("[Macos]Failed to get process path.");
    }
#else
#error "Unsupport system."
#endif
    COMMON_LOG_ERR("Get process path. [%s]", path);
    return path;
}

common::types::fs::PathType CheckPathType(const std::string& path)
{
    try {
        if (!std::filesystem::exists(path)) {
            return common::types::fs::PathType::Nonexistent;
        }
        return std::filesystem::is_directory(path) ? common::types::fs::PathType::Directory
                                                   : common::types::fs::PathType::File;
    } catch (const std::filesystem::filesystem_error& e) {
        COMMON_LOG_ERR("Check path type fail: %s, error code: %d", e.what(), e.code().value());
        return common::types::fs::PathType::Nonexistent;
    }
}

bool CreateDirAndRmOld(const std::string& path)
{
    if (common::utils::filesystem::RemoveDir(path)) {
        return false;
    }
    return stdfs::create_directories(path);
}

bool CreateDirIfNotExist(const std::string& path)
{
    switch (CheckPathType(path)) {
        case common::types::fs::PathType::Directory:
            return true;
            break;
        case common::types::fs::PathType::Nonexistent:
            try {
                return stdfs::create_directories(path);
            } catch (const stdfs::filesystem_error& e) {
                COMMON_LOG_ERR("Failed to create directory: %s, error code: %d", e.what(), e.code().value());
                return false;
            }
        default:
            return false;
            break;
    }
}

bool RemoveDir(const std::string& path)
{
    switch (CheckPathType(path)) {
        case common::types::fs::PathType::Directory:
            try {
                auto n = stdfs::remove_all(path);
                COMMON_LOG_COND((n > 0), "Remove directory %s. n = %u", path.c_str(), n);
                return n > 0;
            } catch (const stdfs::filesystem_error& e) {
                COMMON_LOG_ERR("Failed to remove directory: %s, error code: %d", e.what(), e.code().value());
                return false;
            }
            break;
        case common::types::fs::PathType::File:
            COMMON_LOG_FATAL("Failed to remove directory. %s is file.", path.c_str());
            return false;
            break;
        default:
            COMMON_LOG_INFO("Directory not exist. [%s]", path.c_str());
            return true;
            break;
    }
}

bool IsRegularFile(const std::string& file)
{
    return stdfs::exists(file) && stdfs::is_regular_file(file);
}

std::string RelativeToAbsolutePath(const std::string& relPath, const std::string& basePath)
{
    try {
        stdfs::path base = basePath.empty() ? stdfs::current_path() : stdfs::path(basePath);
        stdfs::path relative(relPath);
        stdfs::path combined = base / relative;
        auto absPath = stdfs::absolute(combined).lexically_normal().string();
        return absPath;
    } catch (const stdfs::filesystem_error& e) {
        COMMON_LOG_ERR("Failed to convert the path {}", e.what());
        return "";
    }
}

std::string GetProcessDir()
{
    std::string process = GetProcessPath();
    stdfs::path proc(process);
    return proc.parent_path().string();
}

}  // namespace common::utils::filesystem
