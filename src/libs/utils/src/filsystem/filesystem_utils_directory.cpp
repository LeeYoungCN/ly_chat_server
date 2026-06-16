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

#if OS_WINDOWS
#include <windows.h>
#elif OS_LINUX
#include <unistd.h>  // Linux的readlink函数
#elif OS_MACOS
#include <mach-o/dyld.h>  // macOS的_NSGetExecutablePath
#endif

#include <exception>
#include <filesystem>

#include "common/debug/debug_logger.h"
#include "common/types/filesystem_types.h"
#include "internal/utils/filesystem_utils_internal.h"
#include "utils/filesystem_utils.h"
#include "utils/thread_utils.h"
#include "utils/utils_error_code.h"

namespace utils::filesystem {

namespace fs = std::filesystem;
using namespace utils::filesystem::internal;

bool dir_exists(std::string_view path)
{
    EntryType type = get_entry_type(path);
    bool result = false;
    switch (type) {
        case EntryType::DIRECTORY:
            result = true;
            set_thread_last_err(ERR_COMM_SUCCESS);
            break;
        case EntryType::NONEXISTENT:
            set_thread_last_err(ERR_UTILS_NOT_FOUND);
            break;
        default:
            set_thread_last_err(ERR_UTILS_NOT_DIRECTORY);
            break;
    }
    return result;
}

bool create_dir(std::string_view path, bool recursive)
{
    auto type = get_entry_type(path);
    if (type == EntryType::DIRECTORY) {
        set_thread_last_err(ERR_UTILS_ALREADY_EXISTS);
        DEBUG_LOGGER_TRACE("[SUCCESS] Create dir: {}, already exist.", path.data());
        return true;
    }
    if (type != EntryType::NONEXISTENT) {
        DEBUG_LOGGER_ERR("[FAILED] Create dir: {}, Target type invalid: {}",
                         path.data(),
                         get_entry_type_str(type));
        set_thread_last_err(ERR_UTILS_NOT_DIRECTORY);
        return false;
    }

    try {
        if (recursive) {
            fs::create_directories(path);
        } else {
            fs::create_directory(path);
        }
        DEBUG_LOGGER_DBG(
            "[SUCCESS] Create dir {}: {}", recursive ? "recursive" : "not recursive", path.data());
        set_thread_last_err(ERR_COMM_SUCCESS);
        return true;
    } catch (const fs::filesystem_error& e) {
        set_thread_last_err(ConvertSysEcToErrorCode(e.code()));
        DEBUG_LOGGER_ERR("[FAILED] Create dir {}: {}, ex: {}",
                         recursive ? "recursive" : "not recursive",
                         path,
                         e.what());
        return false;
    } catch (const std::exception& e) {
        set_thread_last_err(ConvertExceptionToErrorCode(e));
        DEBUG_LOGGER_ERR("[FAILED] Create dir {}: {}, ex: {}",
                         recursive ? "recursive" : "not recursive",
                         path,
                         e.what());
        return false;
    }
}

bool delete_dir(std::string_view path, bool recursive)
{
    if (!dir_exists(path)) {
        bool rst = (get_thread_last_err() == ERR_UTILS_NOT_FOUND);
        DEBUG_LOGGER_COND(
            rst, "Delete dir: {}, message: {}", path.data(), get_thread_last_err_msg());
        return rst;
    }
    try {
        bool result = false;
        if (recursive) {
            result = fs::remove_all(path) > 0;
        } else {
            result = fs::remove(path);  // 非递归删除，目录必须为空
        }
        DEBUG_LOGGER_COND(
            result, "Delete dir {}: {}", recursive ? "recursive" : "not recursive", path.data());
        set_thread_last_err(result ? ERR_COMM_SUCCESS : ERR_UTILS_NOT_FOUND);
        return true;
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOGGER_ERR("[FAILED] Delete dir {}: {}. ex: {}",
                         recursive ? "recursive" : "not recursive",
                         path,
                         e.what());
        set_thread_last_err(ConvertSysEcToErrorCode(e.code()));
        return false;
    } catch (const std::exception& e) {
        DEBUG_LOGGER_ERR("[FAILED] Delete dir {}: {}. ex: {}",
                         recursive ? "recursive" : "not recursive",
                         path,
                         e.what());
        set_thread_last_err(ConvertExceptionToErrorCode(e));
        return false;
    }
}

}  // namespace utils::filesystem
