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

#include "common/compiler/macros.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <unistd.h>  // Linux的readlink函数
#elif PLATFORM_MACOS
#include <mach-o/dyld.h>  // macOS的_NSGetExecutablePath
#endif

#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

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

EntryType GetEntryType(const PathString& path)
{
    if (!fs::exists(path)) {
        SetLastError(ErrorCode::NOT_FOUND);
        return EntryType::NONEXISTENT;
    }
    // 获取文件状态（使用symlink_status而非status，保留符号链接本身的类型）
    std::error_code ec;  // 用于非抛出式错误处理
    fs::file_status status = fs::symlink_status(path, ec);
    if (ec) {
        DEBUG_LOG_ERR("Failed to get symlink status. errCode: %s", ec.value());
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
            DEBUG_LOG_WARN("Unknown entry type: %s", path.c_str());
            return EntryType::UNKNOWN;
    }
}

const char* GetEntryTypeString(EntryType type)
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
#if PLATFORM_WINDOWS
    DWORD length = GetModuleFileNameA(nullptr, path, MAX_PATH_STD);
    if (length == 0 || length >= MAX_PATH_STD) {
        SetLastError(ErrorCode::SYSTEM_ERROR);
        DEBUG_LOG_ERR("[WIN32]Failed to get process path, length: {}", length);
        length = 0;
    }
#elif PLATFORM_LINUX
    auto length = readlink("/proc/self/exe", path, MAX_PATH_STD - 1);
    if (length == -1) {
        SetLastError(ErrorCode::SYSTEM_ERROR);
        DEBUG_LOG_ERR("[Linux]Failed to get process path.");
        length = 0;
    }
    path[length] = '\0';
#elif PLATFORM_MACOS
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        SetLastError(ErrorCode::SYSTEM_ERROR);
        DEBUG_LOG_ERR("[Macos]Failed to get process path.");
    }
#else
#error "Unsupport system."
#endif
    SetLastError(ErrorCode::SUCCESS);
    DEBUG_LOG_DBG("Get process path success: %s", path);
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
        DEBUG_LOG_DBG("Get current working dir successed: %s.", p.string().c_str());
        SetLastError(ErrorCode::SUCCESS);
        return p.string();
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOG_EXCEPTION(e, "Get currtn working dir failed.");
        ConvertSysEcToErrorCode(e.code());
        return "";
    } catch (const std::exception& e) {
        DEBUG_LOG_EXCEPTION(e, "Get currtn working dir failed.");
        ConverExceptionToErrorCode(e);
        return "";
    }
}

// ------------------------------ 路径处理接口 ------------------------------
PathString JoinPaths(const PathList& parts)
{
    if (parts.empty()) {
        DEBUG_LOG_ERR("Path list empty.");
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
        auto normalized=  fs::path(path).lexically_normal();
        DEBUG_LOG_DBG("Normalized path successedd: %s", normalized.string().c_str());
        SetLastError(ErrorCode::SUCCESS);
        return normalized.string();
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOG_EXCEPTION(e, "Normalized path failed");
        ConvertSysEcToErrorCode(e.code());
        return "";
    } catch (const std::exception& e) {
        DEBUG_LOG_EXCEPTION(e, "Normalized path failed");
        ConverExceptionToErrorCode(e);
        return "";
    }
}

PathString ToAbsolutePath(const PathString& relPath, const PathString& baseDir)
{
    if (relPath.empty()) {
        DEBUG_LOG_WARN("")
        return GetCurrentWorkingDirectory();
    }
    fs::path base = fs::path(baseDir.empty() ? GetCurrentWorkingDirectory() : baseDir);
    fs::path relative(relPath);
    fs::path combined = base / relative;

    try {
        auto absPath = fs::absolute(combined).lexically_normal();
        DEBUG_LOG_DBG("Absolute path successed: %s", absPath.c_str());
        SetLastError(ErrorCode::SUCCESS);
        return absPath.string();
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOG_EXCEPTION(e, "Absolute path failed");
        ConvertSysEcToErrorCode(e.code());
        return "";
    } catch (const std::exception& e) {
        DEBUG_LOG_EXCEPTION(e, "Absolute path failed: %s");
        ConverExceptionToErrorCode(e);
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
    auto entrType = GetEntryType(path);
    bool exists = false;
    switch (entrType) {
        case EntryType::FILE:
            exists = true;
            SetLastError(ErrorCode::SUCCESS);
            break;
        case EntryType::NONEXISTENT:
            SetLastError(ErrorCode::NOT_FOUND);
            break;
        default:
            SetLastError(ErrorCode::NOT_FILE);
            break;
    }
    return exists;
}

bool CreateFileUtils(const PathString& path)
{
    EntryType type = GetEntryType(path);
    if (type == EntryType::FILE) {
        SetLastError(ErrorCode::ALREADY_EXISTS);
        DEBUG_LOG_INFO("File already exist: %s", path.c_str());
        return true;
    }
    if (type != EntryType::NONEXISTENT) {
        SetLastError(ErrorCode::NOT_FILE);
        DEBUG_LOG_WARN("Target invalid: %s", GetEntryTypeString(type));
        return false;
    }
    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (file.is_open()) {
        SetLastError(ErrorCode::SUCCESS);
        return true;
    }
    std::error_code ec(errno, std::generic_category());
    ConvertSysEcToErrorCode(ec);
    return false;
}

bool DeleteFileUtils(const PathString& path)
{
    EntryType type = GetEntryType(path);
    if (type == EntryType::NONEXISTENT) {
        DEBUG_LOG_DBG("Delete file successed: %s", path.c_str());
        SetLastError(ErrorCode::NOT_FOUND);
        return true;
    }
    if (type != EntryType::FILE) {
        DEBUG_LOG_ERR("Delete file failed: %s, Type invalid: %s", path.c_str(), GetEntryTypeString(type));
        SetLastError(ErrorCode::NOT_FILE);
        return false;
    }

    try {
        bool result = false;
        result = fs::remove(path);
        DEBUG_LOG_COND(result, "Delete file: %s", path.c_str());
        SetLastError(result ? ErrorCode::SUCCESS : ErrorCode::NOT_FOUND);
        return result;
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOG_EXCEPTION(e, "Delete file failed: %s", path.c_str());
        ConvertSysEcToErrorCode(e.code());
        return false;
    } catch (const std::exception& e) {
        DEBUG_LOG_EXCEPTION(e, "Delete file failed: %s", path.c_str());
        ConverExceptionToErrorCode(e);
        return false;
    }
}

bool CopyFile(const PathString& src, const PathString& dest, bool overwrite)
{
    return false;
}

PathString ReadTextFile(const PathString& path)
{
    return "";
}

types::filesystem::ByteVector ReadBinaryFile(const PathString& path)
{
    return {};
}

bool WriteTextFile(const PathString& path, const PathString& content, bool append)
{
    return false;
}

bool WriteBinaryFile(const PathString& path, const types::filesystem::ByteVector& data, bool append)
{
    return false;
}

types::filesystem::FileSize GetFileSize(const PathString& path)
{
    return {};
}

types::filesystem::FileInfo GetFileInfo(const PathString& path)
{
    return {};
}

bool SetFilePermissions(const PathString& path, types::filesystem::Permission perm)
{
    return false;
}

// ------------------------------ 目录操作接口 ------------------------------

bool DirExists(const PathString& path)
{
    EntryType type = GetEntryType(path);
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
            SetLastError(ErrorCode::NOT_DIRECTORY);
            break;
    }
    return result;
}

bool CreateDir(const PathString& path, bool recursive)
{
    auto type = GetEntryType(path);
    if (type == EntryType::DIRECTORY) {
        SetLastError(ErrorCode::ALREADY_EXISTS);
        DEBUG_LOG_INFO("Create dir successed: %s, alread exist.", path.c_str());
        return true;
    }
    if (type != EntryType::NONEXISTENT) {
        DEBUG_LOG_ERR("Create dir failed: %s, Target type invalid: %s", path.c_str(), GetEntryTypeString(type));
        SetLastError(ErrorCode::NOT_DIRECTORY);
        return false;
    }

    try {
        if (recursive) {
            fs::create_directories(path);
        } else {
            fs::create_directory(path);
        }
        DEBUG_LOG_DBG("Create dir %s success: %s", recursive ? "recursive" : "not recursive", path.c_str());
        SetLastError(ErrorCode::SUCCESS);
        return true;
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOG_EXCEPTION(e, "Create dir %s failed.: %s", recursive ? "recursive" : "not recursive", path.c_str());
        return false;
    } catch (const std::exception& e) {
        ConverExceptionToErrorCode(e);
        DEBUG_LOG_EXCEPTION(e, "Create dir %s failed.: %s", recursive ? "recursive" : "not recursive", path.c_str());
        return false;
    }
}

bool DeleteDir(const PathString& path, bool recursive)
{
    EntryType type = GetEntryType(path);

    if (type == EntryType::NONEXISTENT) {
        DEBUG_LOG_INFO("Delete dir successed, not exits: %s", path.c_str());
        SetLastError(ErrorCode::NOT_FOUND);
        return true;
    }
    if (type != EntryType::DIRECTORY) {
        DEBUG_LOG_ERR("Delete dir failed: %s, Type invalid: %s", path.c_str(), GetEntryTypeString(type));
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
        DEBUG_LOG_COND(result, "Delete dir %s successed: %s", recursive ? "recursive" : "not recursive", path.c_str());
        SetLastError(result ? ErrorCode::SUCCESS : ErrorCode::NOT_FOUND);
        return true;
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOG_EXCEPTION(e, "Delete dir %s failed: %s", recursive ? "recursive" : "not recursive", path.c_str());
        ConvertSysEcToErrorCode(e.code());
        return false;
    } catch (const std::exception& e) {
        DEBUG_LOG_EXCEPTION(e, "Delete dir %s failed: %s", recursive ? "recursive" : "not recursive", path.c_str());
        ConverExceptionToErrorCode(e);
        return false;
    }
}

types::filesystem::PathList ListDir(const PathString& path)
{
    // TODO: 未实现
    return {};
}

std::vector<types::filesystem::FileInfo> ListDirDetailed(const PathString& path)
{
    return {};
}

types::filesystem::FileSize GetDirSize(const PathString& path)
{
    return 0;
}

// ------------------------------ 错误处理接口 ------------------------------
ErrorCode GetLastError()
{
    return GetLastErrorInternal();
}

const char* GetLastErrorString()
{
    return GetErrorString(GetLastErrorInternal());
}

}  // namespace common::utils::filesystem
