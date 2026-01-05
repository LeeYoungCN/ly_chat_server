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

#include "common/common_error_code.h"
#include "common/debug/debug_log.h"
#include "common/types/filesystem_types.h"
#include "common/utils/error_code_utils.h"
#include "common/utils/filesystem_utils.h"
#include "internal/common/utils/filesystem_utils_internal.h"

namespace common::utils::filesystem {

namespace fs = std::filesystem;
using namespace common::constants::filesystem;
using namespace common::types::filesystem;
using namespace common::utils::filesystem::internal;

bool DirExists(std::string_view path)
{
    EntryType type = GetEntryType(path);
    bool result = false;
    switch (type) {
        case EntryType::DIRECTORY:
            result = true;
            set_thread_last_err(ERR_COMM_SUCCESS);
            break;
        case EntryType::NONEXISTENT:
            set_thread_last_err(ERR_COMM_NOT_FOUND);
            break;
        default:
            set_thread_last_err(ERR_COMM_NOT_DIRECTORY);
            break;
    }
    return result;
}

bool CreateDir(std::string_view path, bool recursive)
{
    auto type = GetEntryType(path);
    if (type == EntryType::DIRECTORY) {
        set_thread_last_err(ERR_COMM_ALREADY_EXISTS);
        DEBUG_LOG_DBG("[SUCCESS] Create dir: %s, already exist.", path.data());
        return true;
    }
    if (type != EntryType::NONEXISTENT) {
        DEBUG_LOG_ERR("[FAILED] Create dir: %s, Target type invalid: %s", path.data(), GetEntryTypeString(type));
        set_thread_last_err(ERR_COMM_NOT_DIRECTORY);
        return false;
    }

    try {
        if (recursive) {
            fs::create_directories(path);
        } else {
            fs::create_directory(path);
        }
        DEBUG_LOG_DBG("[SUCCESS] Create dir %s: %s", recursive ? "recursive" : "not recursive", path.data());
        set_thread_last_err(ERR_COMM_SUCCESS);
        return true;
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Create dir %s: %s", recursive ? "recursive" : "not recursive", path.data());
        return false;
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Create dir %s: %s", recursive ? "recursive" : "not recursive", path.data());
        return false;
    }
}

bool DeleteDir(std::string_view path, bool recursive)
{
    if (!DirExists(path)) {
        bool rst = (get_thread_last_err() == ERR_COMM_NOT_FOUND);
        DEBUG_LOG_COND(rst, "Delete dir: %s, message: %s", path.data(), get_thread_last_err_msg());
        return rst;
    }
    try {
        bool result = false;
        if (recursive) {
            result = fs::remove_all(path) > 0;
        } else {
            result = fs::remove(path);  // 非递归删除，目录必须为空
        }
        DEBUG_LOG_COND(result, "Delete dir %s: %s", recursive ? "recursive" : "not recursive", path.data());
        set_thread_last_err(result ? ERR_COMM_SUCCESS : ERR_COMM_NOT_FOUND);
        return true;
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Delete dir %s: %s", recursive ? "recursive" : "not recursive", path.data());
        ConvertSysEcToErrorCode(e.code());
        return false;
    } catch (const std::exception& e) {
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Delete dir %s: %s", recursive ? "recursive" : "not recursive", path.data());
        ConvertExceptionToErrorCode(e);
        return false;
    }
}

}  // namespace common::utils::filesystem
