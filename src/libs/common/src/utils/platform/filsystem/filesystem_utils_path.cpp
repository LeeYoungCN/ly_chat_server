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
        auto normalized = fs::path(path).lexically_normal();
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
        DEBUG_LOG_DBG("Absolute path successed: %s", absPath.string().c_str());
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

}  // namespace common::utils::filesystem
