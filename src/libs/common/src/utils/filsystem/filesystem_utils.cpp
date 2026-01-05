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

#include <filesystem>
#include <system_error>

#include "common/common_error_code.h"
#include "common/debug/debug_log.h"
#include "common/types/filesystem_types.h"
#include "common/utils/error_code_utils.h"
#include "common/utils/filesystem_utils.h"

namespace common::utils::filesystem {

namespace fs = std::filesystem;
using namespace common::constants::filesystem;
using namespace common::types::filesystem;

EntryType GetEntryType(std::string_view path)
{
    if (!fs::exists(path)) {
        SetLastErrcode(ERR_COMM_NOT_FOUND);
        return EntryType::NONEXISTENT;
    }
    // 获取文件状态（使用symlink_status而非status，保留符号链接本身的类型）
    std::error_code ec;  // 用于非抛出式错误处理
    fs::file_status status = fs::symlink_status(path, ec);
    if (ec) {
        DEBUG_LOG_ERR("Failed to get symlink status. errCode: %s", ec.value());
        SetLastErrcode(ERR_COMM_SYSTEM_ERROR);
        return EntryType::UNKNOWN;
    }
    SetLastErrcode(ERR_COMM_SUCCESS);
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
            DEBUG_LOG_WARN("Unknown entry type: %s", path.data());
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

}  // namespace common::utils::filesystem
