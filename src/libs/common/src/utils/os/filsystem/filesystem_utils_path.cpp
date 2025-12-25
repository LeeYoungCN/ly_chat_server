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

#include "common/compiler/macros.h"
#include "common/utils/filesystem_utils.h"

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
#include <string>

#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_log.h"
#include "common/types/filesystem_types.h"
#include "common/utils/filesystem_utils.h"
#include "internal/common/utils/filesystem_utils_internal.h"
#include "common/types/error_code_types.h"
#include "common/utils/error_code_utils.h"
#include "common/common_error_code.h"

namespace common::utils::filesystem {

namespace fs = std::filesystem;
using namespace common::constants::filesystem;
using namespace common::types::filesystem;
using namespace common::utils::filesystem::internal;
using namespace common::types::error_code;
using namespace common::utils::error_code;
using namespace common::error_code;

// ------------------------------ 系统路径接口 ------------------------------
std::string GetProcessPath()
{
    char path[MAX_PATH_STD] = {'\0'};
#if PLATFORM_WINDOWS
    DWORD length = GetModuleFileNameA(nullptr, path, MAX_PATH_STD);
    if (length == 0 || length >= MAX_PATH_STD) {
        SetLastError(ERR_COMM_SYSTEM_ERROR);
        DEBUG_LOG_ERR("[FAILED][WIN32] Get process path, length: %d", length);
        length = 0;
    }
#elif PLATFORM_LINUX
    auto length = readlink("/proc/self/exe", path, MAX_PATH_STD - 1);
    if (length == -1) {
        SetLastError(ERR_COMM_SYSTEM_ERROR);
        DEBUG_LOG_ERR("[FAILED][Linux]Failed to get process path.");
        length = 0;
    }
    path[length] = '\0';
#elif PLATFORM_MACOS
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        SetLastError(ERR_COMM_SYSTEM_ERROR);
        DEBUG_LOG_ERR("[FAILED][Macos]Failed to get process path.");
    }
#else
#error "Unsupport system."
#endif
    SetLastError(ERR_COMM_SUCCESS);
    DEBUG_LOG_DBG("[SUCCESS] Get process path: %s", path);
    return path;
}

PathString GetProcessDirName()
{
    SetLastError(ERR_COMM_SUCCESS);
    return GetDirName(GetProcessPath());
}

PathString GetCurrentWorkingDirectory()
{
    try {
        auto p = fs::current_path();
        DEBUG_LOG_DBG("[SUCCESS] Get current working dir: %s.", p.string().c_str());
        SetLastError(ERR_COMM_SUCCESS);
        return p.string();
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Get current working dir.");
        ConvertSysEcToErrorCode(e.code());
        return "";
    } catch (const std::exception& e) {
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Get current working dir.");
        ConvertExceptionToErrorCode(e);
        return "";
    }
}

// ------------------------------ 路径处理接口 ------------------------------
PathString JoinPaths(const PathList& parts)
{
    if (parts.empty()) {
        SetLastError(ERR_COMM_PATH_INVALID);
        DEBUG_LOG_ERR("[FAILED] Join paths. list empty: %s", GetLastErrorStr());
        return "";
    }
    fs::path result(parts[0]);
    for (size_t i = 1; i < parts.size(); ++i) {
        result /= parts[i];  // std::filesystem 自动处理分隔符
    }
    SetLastError(ERR_COMM_SUCCESS);
    return result.string();
}

PathString NormalizePath(const PathString& path)
{
    try {
        auto normalized = fs::path(path).lexically_normal();
        SetLastError(ERR_COMM_SUCCESS);
        DEBUG_LOG_DBG("[SUCCESS] Normalized path: %s, message: %s", path.c_str(), GetLastErrorStr());
        return normalized.string();
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Normalized path: %s, message: %s", path.c_str(), GetLastErrorStr());
        return "";
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Normalized path: %s, message: %s", path.c_str(), GetLastErrorStr());
        return "";
    }
}

PathString ToAbsolutePath(const PathString& relPath, const PathString& baseDir)
{
    if (relPath.empty()) {
        return GetCurrentWorkingDirectory();
    }
    fs::path base = fs::path(baseDir.empty() ? GetCurrentWorkingDirectory() : baseDir);
    fs::path relative(relPath);
    fs::path combined = base / relative;

    try {
        auto absPath = fs::absolute(combined).lexically_normal();
        SetLastError(ERR_COMM_SUCCESS);
        DEBUG_LOG_DBG("[SUCCESS] Absolute path: %s, message: %s", relPath.c_str(), GetLastErrorStr());
        return absPath.string();
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Absolute path: %s, message: %s", relPath.c_str(), GetLastErrorStr());
        return "";
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Absolute path: %s, message: %s", relPath.c_str(), GetLastErrorStr());
        return "";
    }
}

PathString GetDirName(const PathString& path)
{
    fs::path proc(path);
    SetLastError(ERR_COMM_SUCCESS);
    return proc.parent_path().string();
}

PathString GetBaseName(const PathString& path)
{
    fs::path p(path);
    SetLastError(ERR_COMM_SUCCESS);
    return p.filename().string();
}

PathString GetFileName(const PathString& path)
{
    fs::path p(path);
    SetLastError(ERR_COMM_SUCCESS);
    return p.stem().string();
}

PathString GetExtension(const PathString& path)
{
    fs::path p(path);
    SetLastError(ERR_COMM_SUCCESS);
    return p.extension().string();
}

bool IsAbsolutePath(const PathString& path)
{
    bool result = fs::path(path).is_absolute();
    SetLastError(ERR_COMM_SUCCESS);
    return result;
}

bool IsPathTooLong(const PathString& path)
{
    const auto len = path.length();
    bool result = false;
#if PLATFORM_WINDOWS
    result = len > MAX_PATH_LONG;
#else
    result = len > MAX_PATH_STD;
#endif
    SetLastError(ERR_COMM_SUCCESS);
    return result;
}

}  // namespace common::utils::filesystem
