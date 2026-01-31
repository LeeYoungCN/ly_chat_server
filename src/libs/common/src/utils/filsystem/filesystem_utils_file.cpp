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

#include <chrono>
#include <cstddef>
#include <exception>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>

#include "common/common_error_code.h"
#include "common/debug/debug_logger.h"
#include "common/types/date_time_types.h"
#include "common/types/error_code_types.h"
#include "common/types/filesystem_types.h"
#include "common/utils/error_code_utils.h"
#include "common/utils/filesystem_utils.h"
#include "internal/common/utils/filesystem_utils_internal.h"

// std::ios::out：默认模式，创建文件（若存在则截断）。
// std::ios::app：追加模式（文件存在时在末尾添加内容，不截断）。
// std::ios::binary：二进制模式（避免文本模式的换行符转换）。
// std::ios::trunc 表示如果文件存在则截断它（覆盖原有内容）

namespace {
namespace fs = std::filesystem;
using namespace common::constants::filesystem;
using namespace common::types::filesystem;
using namespace common::utils::filesystem::internal;

common::date_time::TimestampMs GetFileModifyTimestampInternal(std::string_view path)
{
    fs::file_time_type fileTime = fs::last_write_time(path);
    auto sysTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        fileTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(sysTime.time_since_epoch());
    return static_cast<common::date_time::TimestampMs>(ms.count());
}
}  // namespace

namespace common::utils::filesystem {

namespace fs = std::filesystem;
using namespace common::constants::filesystem;
using namespace common::types::filesystem;
using namespace common::utils::filesystem::internal;

bool FileExists(std::string_view path)
{
    auto entrType = GetEntryType(path);
    bool exists = false;
    switch (entrType) {
        case EntryType::FILE:
            exists = true;
            set_thread_last_err(ERR_COMM_SUCCESS);
            break;
        case EntryType::NONEXISTENT:
            set_thread_last_err(ERR_COMM_NOT_FOUND);
            break;
        default:
            set_thread_last_err(ERR_COMM_NOT_FILE);
            break;
    }
    return exists;
}

bool CreateFile(std::string_view path)
{
    EntryType type = GetEntryType(path);
    if (type == EntryType::FILE) {
        set_thread_last_err(ERR_COMM_ALREADY_EXISTS);
        DEBUG_LOGGER_DBG("[SUCCESS] File already exist: {}", path);
        return true;
    }
    if (type != EntryType::NONEXISTENT) {
        set_thread_last_err(ERR_COMM_NOT_FILE);
        DEBUG_LOGGER_ERR("[FAILED] Target invalid: {}", GetEntryTypeString(type));
        return false;
    }

    std::ofstream file(path.data(), std::ios::app | std::ios::binary);
    if (file.is_open()) {
        set_thread_last_err(ERR_COMM_SUCCESS);
        return true;
    }
    std::error_code ec(errno, std::generic_category());
    ConvertSysEcToErrorCode(ec);
    return false;
}

bool DeleteFileInternal(std::string_view path)
{
    try {
        bool result = false;
        result = fs::remove(path);
        DEBUG_LOGGER_COND(result, "Delete file: {}", path);
        set_thread_last_err(result ? ERR_COMM_SUCCESS : ERR_COMM_NOT_FOUND);
        return result;
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOGGER_ERR("[FAILED] Delete file: {}. ex: {}", path, e.what());
        ConvertSysEcToErrorCode(e.code());
        return false;
    } catch (const std::exception& e) {
        DEBUG_LOGGER_ERR("[FAILED] Delete file: {}. ex: {}", path, e.what());
        ConvertExceptionToErrorCode(e);
        return false;
    }
}

bool DeleteFile(std::string_view path)
{
    if (!FileExists(path)) {
        bool rst = (get_thread_last_err() == ERR_COMM_NOT_FOUND);
        DEBUG_LOGGER_COND(rst, "Delete file: {}, message: {}.", path, get_thread_last_err_msg());
        return rst;
    }
    return DeleteFileInternal(path);
}

bool CopyFile(std::string_view src, std::string_view dest, bool overwrite)
{
    if (!FileExists(src)) {
        DEBUG_LOGGER_DBG("[FAILED] Copy file. src : {}, message: {}", src, get_thread_last_err_msg());
        return false;
    }

    EntryType type = GetEntryType(dest);
    type = GetEntryType(dest);
    if (type != EntryType::FILE && type != EntryType::NONEXISTENT) {
        DEBUG_LOGGER_ERR("[FAILED] Dest file: {}, Type invalid: {}", src, GetEntryTypeString(type));
        set_thread_last_err(ERR_COMM_NOT_FILE);
        return false;
    }
    fs::copy_options option = (overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::none);
    try {
        fs::copy_file(src, dest, option);
        set_thread_last_err(ERR_COMM_SUCCESS);
        DEBUG_LOGGER_DBG(
            "[SUCCESS] Copy file {}. src: {}. dst: {}", (overwrite ? "overwrite" : "not overwrite"), src, dest);
        return true;
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOGGER_ERR("[FAILED] Copy file {}. ex: {}.", (overwrite ? "overwrite" : "not overwrite"), e.what());
        return false;
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOGGER_ERR("[FAILED]  Copy file {}. ex: {}.", (overwrite ? "overwrite" : "not overwrite"), e.what());
        return false;
    }
}

bool RenameFile(std::string_view src, std::string_view dest, bool overwrite)
{
    if (!FileExists(src)) {
        DEBUG_LOGGER_ERR("[FAILED] Rename file. src : {}, message: {}", src, get_thread_last_err_msg());
        return false;
    }

    EntryType type = GetEntryType(dest);
    // 类型错误
    if (type != EntryType::FILE && type != EntryType::NONEXISTENT) {
        set_thread_last_err(ERR_COMM_NOT_FILE);
        DEBUG_LOGGER_ERR("[FAILED] Rename file {}. dest invalid: {}, type: {}",
                         (overwrite ? "overwrite " : "not overwrite"),
                         dest,
                         GetEntryTypeString(type));
        return false;
    }
    // 已存在
    if (!overwrite && type == EntryType::FILE) {
        set_thread_last_err(ERR_COMM_ALREADY_EXISTS);
        DEBUG_LOGGER_ERR("[FAILED] Rename file {}. dest already exist: {}, message: {}",
                         (overwrite ? "overwrite " : "not overwrite"),
                         dest,
                         get_thread_last_err_msg());
        return false;
    }

    try {
        fs::rename(src, dest);
        set_thread_last_err(ERR_COMM_SUCCESS);
        DEBUG_LOGGER_DBG("[SUCCESS] Rename file {} succeeded. src: {}, dest: {}, message: {}",
                         (overwrite ? "overwrite " : ""),
                         src,
                         dest,
                         get_thread_last_err_msg());
        return true;
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOGGER_ERR("[FAILED] Rename file {}, src: {}, dest: {}, message: {}. ex: {}.",
                         (overwrite ? "overwrite " : "not overwrite"),
                         src,
                         dest,
                         get_thread_last_err_msg(),
                         e.what());
        return false;
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOGGER_ERR("[FAILED] Rename file {}. src: {}, dest: {}, message: {}. ex: {}",
                         (overwrite ? "overwrite " : "not overwrite"),
                         src,
                         dest,
                         get_thread_last_err_msg(),
                         e.what());
        return false;
    }
}

std::string ReadTextFile(std::string_view path)
{
    if (!FileExists(path)) {
        DEBUG_LOGGER_ERR("[FAILED] Read file: {}, message: {}", path, get_thread_last_err_msg());
        return "";
    }

    std::ifstream file(path.data(), std::ios::in);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOGGER_ERR("[FAILED] Read file: {}, message: {}", path, get_thread_last_err_msg());
        return "";
    }
    DEBUG_LOGGER_DBG("[SUCCESS] Read file: {}, message: {}", path, get_thread_last_err_msg());
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

types::filesystem::ByteVector ReadBinaryFile(std::string_view path)
{
    if (!FileExists(path)) {
        DEBUG_LOGGER_ERR("[FAILED] Read binary file: {}, message: {}", path, get_thread_last_err_msg());
        return {};
    }

    std::ifstream file(path.data(), std::ios::in);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOGGER_ERR("[FAILED] Read binary file: {}, message: {}", path, get_thread_last_err_msg());
        return {};
    }
    FileSize fileSize = fs::file_size(path);
    ByteVector buffer(fileSize);
    file.read(reinterpret_cast<char*>(buffer.data()), static_cast<int64_t>(fileSize));
    if (static_cast<size_t>(file.gcount()) != fileSize) {
        return {};
    }
    return buffer;
}

bool WriteTextFile(std::string_view path, std::string_view content, bool overwrite)
{
    if (!FileExists(path)) {
        DEBUG_LOGGER_ERR("[FAILED] Write to text file: {}, message: {}", path, get_thread_last_err_msg());
        return false;
    }
    std::ios::openmode mode = std::ios::out;
    if (overwrite) {
        mode = (std::ios::out | std::ios::trunc);
    } else {
        mode = (std::ios::out | std::ios::app);
    }
    std::ofstream file(path.data(), mode);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOGGER_ERR("[FAILED] Write text file {}: {}, message: {}",
                         (overwrite ? "overwrite" : "append"),
                         path,
                         get_thread_last_err_msg());
        return false;
    }

    if (file << content) {
        file.close();
        set_thread_last_err(ERR_COMM_SUCCESS);
        DEBUG_LOGGER_DBG("[SUCCESS] Write text file {}: {}, message: {}",
                         (overwrite ? "overwrite" : "append"),
                         path,
                         get_thread_last_err_msg());
        return true;
    }

    std::error_code ec(errno, std::system_category());
    ConvertSysEcToErrorCode(ec);
    DEBUG_LOGGER_ERR("[FAILED] Write text file {}: {}, message: {}",
                     (overwrite ? "overwrite" : "append"),
                     path,
                     get_thread_last_err_msg());
    return false;
}

FileSize GetFileSize(std::string_view path)
{
    if (!FileExists(path)) {
        DEBUG_LOGGER_ERR("[FAILED] Get file size: {}, message: {}", path, get_thread_last_err_msg());
        return 0;
    }
    std::error_code ec;
    const auto& size = fs::file_size(path, ec);
    ConvertSysEcToErrorCode(ec);
    return size;
}

types::filesystem::FileInfo GetFileInfo(std::string_view path)
{
    FileInfo fileInfo{};
    fileInfo.path = path;
    fileInfo.type = GetEntryType(path);

    if (fileInfo.type == EntryType::NONEXISTENT) {
        return fileInfo;
    }
    if (fileInfo.type == EntryType::FILE) {
        fileInfo.size = fs::file_size(path);
    }
    fileInfo.modifyTime = GetFileModifyTimestampInternal(path);

    return fileInfo;
}

}  // namespace common::utils::filesystem
