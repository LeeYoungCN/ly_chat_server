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
#include <iterator>
#include <string>
#include <system_error>

#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_log.h"
#include "common/types/filesystem_types.h"
#include "common/utils/filesystem_utils.h"
#include "internal/common/utils/filesystem_utils_internal.h"

// std::ios::out：默认模式，创建文件（若存在则截断）。
// std::ios::app：追加模式（文件存在时在末尾添加内容，不截断）。
// std::ios::binary：二进制模式（避免文本模式的换行符转换）。
// std::ios::trunc 表示如果文件存在则截断它（覆盖原有内容）

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

    std::ofstream file(path, std::ios::app | std::ios::binary);
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
    if (!FileExists(path)) {
        bool rst = (GetLastError() == ErrorCode::NOT_FOUND);
        DEBUG_LOG_COND(rst, "Delete file: %s, message: %s.", path.c_str(), GetLastErrorString());
        return rst;
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

bool CopyFileUtils(const PathString& src, const PathString& dest, bool overwrite)
{
    EntryType type = GetEntryType(src);
    if (type == EntryType::NONEXISTENT) {
        DEBUG_LOG_ERR("Src file nonexist: %s", src.c_str());
        SetLastError(ErrorCode::NOT_FOUND);
        return false;
    } else if (type != EntryType::FILE) {
        DEBUG_LOG_ERR("Src file: %s, Type invalid: %s", src.c_str(), GetEntryTypeString(type));
        SetLastError(ErrorCode::NOT_FILE);
        return false;
    }

    type = GetEntryType(dest);
    if (type != EntryType::FILE && type != EntryType::NONEXISTENT) {
        DEBUG_LOG_ERR("Dest file: %s, Type invalid: %s", src.c_str(), GetEntryTypeString(type));
        SetLastError(ErrorCode::NOT_FILE);
        return false;
    }
    fs::copy_options option = (overwrite ? fs::copy_options::overwrite_existing : fs::copy_options::none);
    try {
        fs::copy_file(src, dest, option);
        SetLastError(ErrorCode::SUCCESS);
        DEBUG_LOG_DBG(
            "Copy file %s successd. src: %s. dst: %s", (overwrite ? "overweite" : "none"), src.c_str(), dest.c_str());
        return true;
    } catch (const fs::filesystem_error& e) {
        ConvertSysEcToErrorCode(e.code());
        DEBUG_LOG_EXCEPTION(e, "Copy file %s failed.", (overwrite ? "overweite" : "none"));
        return false;
    } catch (const std::exception& e) {
        ConverExceptionToErrorCode(e);
        DEBUG_LOG_EXCEPTION(e, "Copy file %s failed.", (overwrite ? "overweite" : "none"));
        return false;
    }
}

std::string ReadTextFile(const PathString& path)
{
    if (!FileExists(path)) {
        DEBUG_LOG_ERR("Read file failed: %s, message: %s", path.c_str(), GetLastErrorString());
        return "";
    }

    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOG_ERR("Read file failed: %s, message: %s", path.c_str(), GetLastErrorString());
        return "";
    }
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

types::filesystem::ByteVector ReadBinaryFile(const PathString& path)
{
    if (!FileExists(path)) {
        DEBUG_LOG_ERR("Read binary file failed: %s, message: %s", path.c_str(), GetLastErrorString());
        return {};
    }

    std::ifstream file(path, std::ios::in);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOG_ERR("Read binary file failed: %s, message: %s", path.c_str(), GetLastErrorString());
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

bool WriteTextFile(const PathString& path, const PathString& content, bool append)
{
    if (!FileExists(path)) {
        DEBUG_LOG_ERR("Write to text file failed: %s, message: %s", path.c_str(), GetLastErrorString());
        return false;
    }
    std::ios::openmode mode = std::ios::out;
    if (append) {
        mode = (std::ios::out | std::ios::app);
    } else {
        mode = (std::ios::out | std::ios::trunc);
    }
    std::ofstream file(path, mode);
    if (!file.is_open()) {
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOG_ERR("Write text file failed: %s, message: %s", path.c_str(), GetLastErrorString());
        return false;
    }

    file << content;

    file.close();
    SetLastError(ErrorCode::SUCCESS);
    DEBUG_LOG_DBG("Write text file successed: %s, message: %s", path.c_str(), GetLastErrorString());
    return true;
}

bool WriteBinaryFile(const PathString& path, const types::filesystem::ByteVector& data, bool append)
{
    return false;
}

types::filesystem::FileSize GetFileSize(const PathString& path)
{
    if (!FileExists(path)) {
        DEBUG_LOG_ERR("Get file size failed: %s, message: %s", path.c_str(), GetLastErrorString());
        return 0;
    }
    std::error_code ec;
    const auto& size = fs::file_size(path, ec);
    ConvertSysEcToErrorCode(ec);
    return size;
}

types::filesystem::FileInfo GetFileInfo(const PathString& path)
{
    return {};
}

bool SetFilePermissions(const PathString& path, types::filesystem::Permission perm)
{
    return false;
}

}  // namespace common::utils::filesystem
