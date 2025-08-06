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
#include <iterator>
#include <string>
#include <system_error>

#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_log.h"
#include "common/types/date_time_types.h"
#include "common/types/filesystem_types.h"
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

common::types::date_time::TimestampMs GetFileModifyTimestampInternal(const PathString& path)
{
    fs::file_time_type fileTime = fs::last_write_time(path);
    auto sysTime = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        fileTime - std::filesystem::file_time_type::clock::now() + std::chrono::system_clock::now());
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(sysTime.time_since_epoch());
    return static_cast<common::types::date_time::TimestampMs>(ms.count());
}
}  // namespace

namespace common::utils::filesystem {

namespace fs = std::filesystem;
using namespace common::constants::filesystem;
using namespace common::types::filesystem;
using namespace common::utils::filesystem::internal;

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

bool CreateFile(const PathString& path)
{
    EntryType type = GetEntryType(path);
    if (type == EntryType::FILE) {
        SetLastError(ErrorCode::ALREADY_EXISTS);
        DEBUG_LOG_DBG("[SUCCESS] File already exist: %s", path.c_str());
        return true;
    }
    if (type != EntryType::NONEXISTENT) {
        SetLastError(ErrorCode::NOT_FILE);
        DEBUG_LOG_ERR("[FAILED] Target invalid: %s", GetEntryTypeString(type));
        return false;
    }

    std::ofstream file(path, std::ios::app | std::ios::binary);
    if (file.is_open()) {
        SetLastError(ErrorCode::SUCCESS);
        return true;
    }
    std::error_code ec(errno, std::generic_category());
    ConvertSysEcToErrorCode(ec);
    return false;
}

bool DeleteFileInternal(const PathString& path)
{
    try {
        bool result = false;
        result = fs::remove(path);
        DEBUG_LOG_COND(result, "Delete file: %s", path.c_str());
        SetLastError(result ? ErrorCode::SUCCESS : ErrorCode::NOT_FOUND);
        return result;
    } catch (const fs::filesystem_error& e) {
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Delete file: %s", path.c_str());
        ConvertSysEcToErrorCode(e.code());
        return false;
    } catch (const std::exception& e) {
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Delete file: %s", path.c_str());
        ConvertExceptionToErrorCode(e);
        return false;
    }
}

bool DeleteFile(const PathString& path)
{
    if (!FileExists(path)) {
        bool rst = (GetLastError() == ErrorCode::NOT_FOUND);
        DEBUG_LOG_COND(rst, "Delete file: %s, message: %s.", path.c_str(), GetLastErrorString());
        return rst;
    }
    return DeleteFileInternal(path);
}

bool CopyFile(const PathString& src, const PathString& dest, bool overwrite)
{
    if (!FileExists(src)) {
        DEBUG_LOG_DBG("[FAILED] Copy file. src : %s, message: %s", src.c_str(), GetLastErrorString());
        return false;
    }

    EntryType type = GetEntryType(dest);
    type = GetEntryType(dest);
    if (type != EntryType::FILE && type != EntryType::NONEXISTENT) {
        DEBUG_LOG_ERR("[FAILED] Dest file: %s, Type invalid: %s", src.c_str(), GetEntryTypeString(type));
        SetLastError(ErrorCode::NOT_FILE);
        return false;
    }
    fs::copy_options option = (overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::none);
    try {
        fs::copy_file(src, dest, option);
        SetLastError(ErrorCode::SUCCESS);
        DEBUG_LOG_DBG("[SUCCESS] Copy file %s. src: %s. dst: %s",
                      (overwrite ? "overwrite" : "not overwrite"),
                      src.c_str(),
                      dest.c_str());
        return true;
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOG_EXCEPTION(e, "[FAILED] Copy file %s.", (overwrite ? "overwrite" : "not overwrite"));
        return false;
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOG_EXCEPTION(e, "[FAILED]  Copy file %s.", (overwrite ? "overwrite" : "not overwrite"));
        return false;
    }
}

bool RenameFile(const PathString& src, const PathString& dest, bool overwrite)
{
    if (!FileExists(src)) {
        DEBUG_LOG_ERR("[FAILED] Rename file. src : %s, message: %s", src.c_str(), GetLastErrorString());
        return false;
    }

    EntryType type = GetEntryType(dest);
    // 类型错误
    if (type != EntryType::FILE && type != EntryType::NONEXISTENT) {
        SetLastError(ErrorCode::NOT_FILE);
        DEBUG_LOG_ERR("[FAILED] Rename file %s. dest invalid: %s, type: %s",
                      (overwrite ? "overwrite " : "not overwrite"),
                      dest.c_str(),
                      GetEntryTypeString(type));
        return false;
    }
    // 已存在
    if (!overwrite && type == EntryType::FILE) {
        SetLastError(ErrorCode::ALREADY_EXISTS);
        DEBUG_LOG_ERR("[FAILED] Rename file %s. dest already exist: %s, message: %s",
                      (overwrite ? "overwrite " : "not overwrite"),
                      dest.c_str(),
                      GetLastErrorString());
        return false;
    }

    try {
        fs::rename(src, dest);
        SetLastError(ErrorCode::SUCCESS);
        DEBUG_LOG_DBG("[SUCCESS] Rename file %s succeeded. src: %s, dest: %s, message: %s",
                      (overwrite ? "overwrite " : ""),
                      src.c_str(),
                      dest.c_str(),
                      GetLastErrorString());
        return true;
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOG_EXCEPTION(e,
                            "[FAILED] Rename file %s, src: %s, dest: %s, message: %s",
                            (overwrite ? "overwrite " : "not overwrite"),
                            src.c_str(),
                            dest.c_str(),
                            GetLastErrorString());
        return false;
    } catch (const std::exception& e) {
        ConvertExceptionToErrorCode(e);
        DEBUG_LOG_EXCEPTION(e,
                            "[FAILED] Rename file %s. src: %s, dest: %s, message: %s",
                            (overwrite ? "overwrite " : "not overwrite"),
                            src.c_str(),
                            dest.c_str(),
                            GetLastErrorString());
        return false;
    }
}

std::string ReadTextFile(const PathString& path)
{
    if (!FileExists(path)) {
        DEBUG_LOG_ERR("[FAILED] Read file: %s, message: %s", path.c_str(), GetLastErrorString());
        return "";
    }

    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOG_ERR("[FAILED] Read file: %s, message: %s", path.c_str(), GetLastErrorString());
        return "";
    }
    DEBUG_LOG_DBG("[SUCCESS] Read file: %s, message: %s", path.c_str(), GetLastErrorString());
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

types::filesystem::ByteVector ReadBinaryFile(const PathString& path)
{
    if (!FileExists(path)) {
        DEBUG_LOG_ERR("[FAILED] Read binary file: %s, message: %s", path.c_str(), GetLastErrorString());
        return {};
    }

    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOG_ERR("[FAILED] Read binary file: %s, message: %s", path.c_str(), GetLastErrorString());
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

bool WriteTextFile(const PathString& path, const PathString& content, bool overwrite)
{
    if (!FileExists(path)) {
        DEBUG_LOG_ERR("[FAILED] Write to text file: %s, message: %s", path.c_str(), GetLastErrorString());
        return false;
    }
    std::ios::openmode mode = std::ios::out;
    if (overwrite) {
        mode = (std::ios::out | std::ios::trunc);
    } else {
        mode = (std::ios::out | std::ios::app);
    }
    std::ofstream file(path, mode);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOG_ERR("[FAILED] Write text file %s: %s, message: %s",
                      (overwrite ? "append" : "overwrite"),
                      path.c_str(),
                      GetLastErrorString());
        return false;
    }

    if (file << content) {
        file.close();
        SetLastError(ErrorCode::SUCCESS);
        DEBUG_LOG_DBG("[SUCCESS] Write text file %s: %s, message: %s",
                      (overwrite ? "append" : "overwrite"),
                      path.c_str(),
                      GetLastErrorString());
        return true;
    }

    std::error_code ec(errno, std::system_category());
    ConvertSysEcToErrorCode(ec);
    DEBUG_LOG_ERR("[FAILED] Write text file %s: %s, message: %s",
                  (overwrite ? "append" : "overwrite"),
                  path.c_str(),
                  GetLastErrorString());
    return false;
}

FileSize GetFileSize(const PathString& path)
{
    if (!FileExists(path)) {
        DEBUG_LOG_ERR("[FAILED] Get file size: %s, message: %s", path.c_str(), GetLastErrorString());
        return 0;
    }
    std::error_code ec;
    const auto& size = fs::file_size(path, ec);
    ConvertSysEcToErrorCode(ec);
    return size;
}

types::filesystem::FileInfo GetFileInfo(const PathString& path)
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
