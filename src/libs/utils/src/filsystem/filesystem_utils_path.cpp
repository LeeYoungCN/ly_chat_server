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
#include "utils/filesystem_utils.h"

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
#include "common/debug/debug_logger.h"
#include "common/types/error_code_types.h"
#include "common/types/filesystem_types.h"
#include "internal/utils/filesystem_utils_internal.h"
#include "utils/filesystem_utils.h"
#include "utils/thread_utils.h"
#include "utils/utils_error_code.h"

namespace utils::filesystem {
namespace fs = std::filesystem;
using namespace utils::filesystem::internal;
using namespace constants::filesystem;

// ------------------------------ 系统路径接口 ------------------------------

std::string GetCurrentWorkingDirectory()
{
    try {
        auto p = fs::current_path();
        DEBUG_LOGGER_DBG("[SUCCESS] Get current working dir: {}.", p.string().c_str());
        set_thread_last_err(ERR_COMM_SUCCESS);
        return p.string();
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOGGER_ERR("[FAILED] Get current working dir.");
        ConvertSysEcToErrorCode(e.code());
        return "";
    } catch (const std::exception& e) {
        DEBUG_LOGGER_ERR("[FAILED] Get current working dir.");
        ConvertExceptionToErrorCode(e);
        return "";
    }
}

// ------------------------------ 路径处理接口 ------------------------------
std::string JoinPaths(const PathList& parts)
{
    if (parts.empty()) {
        set_thread_last_err(ERR_UTILS_PATH_INVALID);
        DEBUG_LOGGER_ERR("[FAILED] Join paths. list empty: {}", get_thread_last_err_msg());
        return "";
    }
    fs::path result(parts[0]);
    for (size_t i = 1; i < parts.size(); ++i) {
        result /= parts[i];  // std::filesystem 自动处理分隔符
    }
    set_thread_last_err(ERR_COMM_SUCCESS);
    return result.string();
}

std::string NormalizePath(std::string_view path)
{
    try {
        auto normalized = fs::path(path).lexically_normal();
        set_thread_last_err(ERR_COMM_SUCCESS);
        DEBUG_LOGGER_DBG(
            "[SUCCESS] Normalized path: {}, message: {}", path, get_thread_last_err_msg());
        return normalized.string();
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOGGER_ERR("[FAILED] Normalized path: {}, message: {}. ex: {}",
                         path,
                         get_thread_last_err_msg(),
                         e.what());
        return "";
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOGGER_ERR("[FAILED] Normalized path: {}, message: {}. ex: {}",
                         path,
                         get_thread_last_err_msg(),
                         e.what());
        return "";
    }
}

std::string ToAbsolutePath(std::string_view relPath, std::string_view baseDir)
{
    if (relPath.empty()) {
        return GetCurrentWorkingDirectory();
    }
    fs::path base = fs::path(baseDir.empty() ? GetCurrentWorkingDirectory() : baseDir);
    fs::path relative(relPath);
    fs::path combined = base / relative;

    try {
        auto absPath = fs::absolute(combined).lexically_normal();
        set_thread_last_err(ERR_COMM_SUCCESS);
        DEBUG_LOGGER_DBG(
            "[SUCCESS] Absolute path: {}, message: {}", relPath, get_thread_last_err_msg());
        return absPath.string();
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOGGER_ERR("[FAILED] Absolute path: {}, message: {}. ex: {}",
                         relPath,
                         get_thread_last_err_msg(),
                         e.what());
        return "";
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOGGER_ERR("[FAILED] Absolute path: {}, message: {}.ex: {}",
                         relPath,
                         get_thread_last_err_msg(),
                         e.what());
        return "";
    }
}

std::string GetDirectory(std::string_view path)
{
    fs::path proc(path);
    set_thread_last_err(ERR_COMM_SUCCESS);
    return proc.parent_path().string();
}

std::string GetBaseName(std::string_view path)
{
    fs::path p(path);
    set_thread_last_err(ERR_COMM_SUCCESS);
    return p.filename().string();
}

std::string GetFileName(std::string_view path)
{
    fs::path p(path);
    set_thread_last_err(ERR_COMM_SUCCESS);
    return p.stem().string();
}

std::string GetExtension(std::string_view path)
{
    fs::path p(path);
    set_thread_last_err(ERR_COMM_SUCCESS);
    return p.extension().string();
}

bool IsAbsolutePath(std::string_view path)
{
    bool result = fs::path(path).is_absolute();
    set_thread_last_err(ERR_COMM_SUCCESS);
    return result;
}

bool IsPathTooLong(std::string_view path)
{
    const auto len = path.length();
    bool result = false;
#if PLATFORM_WINDOWS
    result = len > MAX_PATH_LONG;
#else
    result = len > MAX_PATH_STD;
#endif
    set_thread_last_err(ERR_COMM_SUCCESS);
    return result;
}

}  // namespace utils::filesystem
