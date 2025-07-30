/**
 * @file filesystem_utils.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-30
 *
 * @copyright Copyright (c) 2025
 *
 */
#include "common/utils/filesystem_utils.h"

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>  // Linux的readlink函数
#elif defined(__APPLE__)
#include <mach-o/dyld.h>  // macOS的_NSGetExecutablePath
#endif

#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>

#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_log.h"
#include "common/types/filesystem_types.h"
#include "common/utils/filesystem_utils.h"
#include "internal/common/utils/filesystem_utils_internal.h"

namespace common::utils::filesystem {

namespace fs = std::filesystem;
using namespace common::constants::filesystem;
using namespace common::types::filesystem;
using namespace common::utils::filesystem::internal;

EntryType GetEtryType(const PathString& path)
{
    if (!fs::exists(path)) {
        SetLastError(ErrorCode::NOT_FOUND);
        return EntryType::NONEXISTENT;
    }
    // 获取文件状态（使用symlink_status而非status，保留符号链接本身的类型）
    std::error_code ec;  // 用于非抛出式错误处理
    fs::file_status status = fs::symlink_status(path, ec);
    if (ec) {
        COMMON_LOG_ERR("Failed to get symlink status. errCode: %s", ec.value());
        SetLastError(ErrorCode::SYSTEM_ERROR);
        return EntryType::UNKNOWN;
    }
    SetLastError(ErrorCode::SUCCESS);
    switch (status.type()) {
        case fs::file_type::regular:  // 普通文件
            return EntryType::FILE;
        case fs::file_type::directory:
            return EntryType::DIRECTORY;
        case fs::file_type::symlink:
            return EntryType::SYMBOL_LINK;
        case fs::file_type::character:  // 字符设备
            return EntryType::CHARACTER_DEVICE;
        case fs::file_type::block:
            return EntryType::BLOCK_DEVICE;
        case fs::file_type::fifo:
            return EntryType::SOCKET;
        case fs::file_type::unknown:
        default:
            COMMON_LOG_WARN("Unknown entry type: %s", path.c_str());
            return EntryType::UNKNOWN;
    }
}

std::string_view GetEntryTypeString(EntryType type)
{
    switch (type) {
        case EntryType::NONEXISTENT:
            return "Nonexistent";
        case EntryType::FILE:
            return "File";
        case EntryType::DIRECTORY:
            return "Directory";
        case EntryType::SYMBOL_LINK:
            return "Symbol link";
        case EntryType::BLOCK_DEVICE:
            return "Block device";
        case EntryType::CHARACTER_DEVICE:
            return "Character Device";
        case EntryType::FIFO:
            return "FIFO";
        case EntryType::SOCKET:
            return "Socket";
        case EntryType::UNKNOWN:
        default:
            return "Unknown";
    }
}

// ------------------------------ 系统路径接口 ------------------------------
std::string GetProcessPath()
{
    char path[MAX_PATH_STD] = {'\0'};
#ifdef _WIN32
    DWORD length = GetModuleFileNameA(nullptr, path, MAX_PATH_STD);
    if (length == 0 || length >= MAX_PATH_STD) {
        SetLastError(ErrorCode::SYSTEM_ERROR);
        COMMON_LOG_ERR("[WIN32]Failed to get process path, length: {}", length);
        length = 0;
    }
#elif defined(__linux__)
    auto length = readlink("/proc/self/exe", path, MAX_PATH_STD - 1);
    if (length == -1) {
        SetLastError(ErrorCode::SYSTEM_ERROR);
        COMMON_LOG_ERR("[Linux]Failed to get process path.");
        length = 0;
    }
    path[length] = '\0';
#elif defined(__APPLE__)
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        SetLastError(ErrorCode::SYSTEM_ERROR);
        COMMON_LOG_ERR("[Macos]Failed to get process path.");
    }
#else
#error "Unsupport system."
#endif
    SetLastError(ErrorCode::SUCCESS);
    return path;
}

PathString GetProcessDirName()
{
    SetLastError(ErrorCode::SUCCESS);
    return GetDirName(GetProcessPath());
}

PathString GetCurrentWorkingDirectory()
{
    try {
        auto p = fs::current_path();
        SetLastError(ErrorCode::SUCCESS);
        return p.string();
    } catch (const fs::filesystem_error& e) {
        COMMON_LOG_EXCEPTION(e, "Faied to get current working directory: %s");
        SetLastError(ErrorCode::SYSTEM_ERROR);
        return "";
    }
}

// ------------------------------ 路径处理接口 ------------------------------
PathString JoinPaths(const PathList& parts)
{
    if (parts.empty()) {
        COMMON_LOG_ERR("Path list empty.");
        SetLastError(ErrorCode::NOT_FOUND);
        return "";
    }
    fs::path result(parts[0]);
    for (size_t i = 1; i < parts.size(); ++i) {
        result /= parts[i];  // std::filesystem 自动处理分隔符
    }
    SetLastError(ErrorCode::SUCCESS);
    return result.string();
}

PathString NormalizePath(const PathString& path)
{
    try {
        auto normalized = fs::weakly_canonical(path);
        SetLastError(ErrorCode::SUCCESS);
        return normalized.string();
    } catch (const std::exception& e) {
        COMMON_LOG_EXCEPTION(e, "Failed to normalize path: %s", path.c_str());
        SetLastError(ErrorCode::PATH_TOO_LONG);
        return "";
    }
}

PathString ToAbsolutePath(const PathString& relPath, const PathString& baseDir)
{
    fs::path base = baseDir.empty() ? fs::current_path() : fs::path(baseDir);
    fs::path relative(relPath);
    fs::path combined = base / relative;
    try {
        auto absPath = fs::absolute(combined).lexically_normal().string();
        SetLastError(ErrorCode::SUCCESS);
        return absPath;
    } catch (const fs::filesystem_error& e) {
        COMMON_LOG_EXCEPTION(e, "Failed to convert the path: %s", relPath.c_str());
        SetLastError(ErrorCode::PATH_TOO_LONG);
        return "";
    }
}

PathString GetDirName(const PathString& path)
{
    fs::path proc(path);
    SetLastError(ErrorCode::SUCCESS);
    return proc.parent_path().string();
}

PathString GetBaseName(const PathString& path)
{
    fs::path p(path);
    SetLastError(ErrorCode::SUCCESS);
    return p.filename().string();
}

PathString GetFileName(const PathString& path)
{
    fs::path p(path);
    SetLastError(ErrorCode::SUCCESS);
    return p.stem().string();
}

PathString GetExtension(const PathString& path)
{
    fs::path p(path);
    SetLastError(ErrorCode::SUCCESS);
    return p.extension().string();
}

bool IsAbsolutePath(const PathString& path)
{
    bool result = fs::path(path).is_absolute();
    SetLastError(ErrorCode::SUCCESS);
    return result;
}

bool IsPathTooLong(const PathString& path)
{
    const auto len = path.length();
    bool result = false;
#ifdef _WIN32
    result = len > MAX_PATH_LONG;
#else
    result = len > MAX_PATH_STD;
#endif
    SetLastError(ErrorCode::SUCCESS);
    return result;
}

// ------------------------------ 文件操作接口 ------------------------------
bool FileExists(const PathString& path)
{
    auto entrType = GetEtryType(path);
    bool exists = false;
    switch (entrType) {
        case EntryType::FILE:
            exists = true;
            SetLastError(ErrorCode::SUCCESS);
            break;
        case EntryType::NONEXISTENT:
            SetLastError(ErrorCode::NOT_FOUND);
            COMMON_LOG_ERR("File not exist: %s", path.c_str());
            break;
        default:
            SetLastError(ErrorCode::NOT_FILE);
            COMMON_LOG_ERR("File is %s: %s", GetEntryTypeString(entrType), path.c_str());
            break;
    }
    return exists;
}

bool CreateFile(const PathString& path)
{
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        SetLastError(ErrorCode::SUCCESS);
        return true;
    }

    return false;
}

// ------------------------------ 目录操作接口 ------------------------------

bool DirExists(const PathString& path)
{
    EntryType type = GetEtryType(path);
    bool result = false;
    switch (type) {
        case EntryType::DIRECTORY:
            result = true;
            SetLastError(ErrorCode::SUCCESS);
            break;
        case EntryType::NONEXISTENT:
            SetLastError(ErrorCode::NOT_FOUND);
            break;
        default:
            COMMON_LOG_ERR("Target type invalid: %s", GetEntryTypeString(type));
            SetLastError(ErrorCode::NOT_DIRECTORY);
            break;
    }
    return result;
}

bool CreateDir(const PathString& path, bool recursive)
{
    auto type = GetEtryType(path);
    if (type == EntryType::DIRECTORY) {
        SetLastError(ErrorCode::SUCCESS);
        COMMON_LOG_INFO("Dir already exist: %s", path.c_str());
        return true;
    }
    if (type != EntryType::NONEXISTENT) {
        COMMON_LOG_ERR("Target type invalid: %s", GetEntryTypeString(type));
        SetLastError(ErrorCode::NOT_DIRECTORY);
        return false;
    }
    try {
        if (recursive) {
            fs::create_directories(path);
        } else {
            fs::create_directory(path);
        }
        SetLastError(ErrorCode::SUCCESS);
        return true;
    } catch (const std::exception& e) {
        COMMON_LOG_EXCEPTION(e, "Failed to create dir %s.", recursive ? "recursive" : "not recursive");
        SetLastError(ErrorCode::PERMISSION_DENIED);
        return false;
    }
}

bool DeleteDir(const PathString& path, bool recursive)
{
    EntryType type = GetEtryType(path);

    if (type == EntryType::NONEXISTENT) {
        COMMON_LOG_INFO("Delete dir success, not exits: %s", path.c_str());
        SetLastError(ErrorCode::NOT_FOUND);
        return true;
    }
    if (type != EntryType::DIRECTORY) {
        COMMON_LOG_ERR("Failed to delete dir %s, Type: %s", path.c_str(), GetEntryTypeString(type));
        SetLastError(ErrorCode::NOT_DIRECTORY);
        return false;
    }
    try {
        bool result = false;
        if (recursive) {
            result = fs::remove_all(path) > 0;
        } else {
            result = fs::remove(path);  // 非递归删除，目录必须为空
        }
        SetLastError(result ? ErrorCode::SUCCESS : ErrorCode::NOT_FOUND);
        COMMON_LOG_COND(result, "Remove directory %s : %s", recursive ? "recursive" : "not recursive", path.c_str());
        return true;
    } catch (const std::exception& e) {
        COMMON_LOG_EXCEPTION(e, "Failed to delete dir %s", recursive ? "recursive" : "not recursive");
        SetLastError(ErrorCode::PERMISSION_DENIED);
        return false;
    }
}

// ------------------------------ 错误处理接口 ------------------------------
ErrorCode GetLastError()
{
    return g_fileSystemLastError;
}

std::string_view GetErrorString(constants::filesystem::ErrorCode code)
{
    switch (code) {
        case constants::filesystem::ErrorCode::SUCCESS:
            return "Success";
        case constants::filesystem::ErrorCode::NOT_FOUND:
            return "Not found";
        case constants::filesystem::ErrorCode::PERMISSION_DENIED:
            return "Permissin denied";
        case constants::filesystem::ErrorCode::PATH_TOO_LONG:
            return "Path too long";
        case constants::filesystem::ErrorCode::ALREADY_EXISTS:
            return "Target already exists";
        case constants::filesystem::ErrorCode::NOT_DIRECTORY:
            return "Not directory";
        case constants::filesystem::ErrorCode::NOT_FILE:
            return "Not file";
        case constants::filesystem::ErrorCode::IO_ERROR:
            return "I/O error";
        case constants::filesystem::ErrorCode::SYSTEM_ERROR:
            return "System error";
        default:
            return "Unknown error code";
    }
}

}  // namespace common::utils::filesystem
