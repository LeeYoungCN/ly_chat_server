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

#include <exception>
#include <filesystem>

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
        DEBUG_LOG_EXCEPTION(e, "Create dir %s failed: %s", recursive ? "recursive" : "not recursive", path.c_str());
        return false;
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOG_EXCEPTION(e, "Create dir %s failed: %s", recursive ? "recursive" : "not recursive", path.c_str());
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
        ConvertExceptionToErrorCode(e);
        return false;
    }
}

}  // namespace common::utils::filesystem
