#include "internal/common/utils/filesystem_utils_internal.h"

#include "common/compiler/macros.h"

#if COMPILER_MSVC
#include <windows.h>
#else
#include <cerrno>
#endif

#include <exception>
#include <filesystem>
#include <system_error>
#include <unordered_map>

#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_log.h"
#include "internal/common/utils/filesystem_utils_internal.h"

namespace {
thread_local volatile common::constants::filesystem::ErrorCode g_fileSystemLastError =
    common::constants::filesystem::ErrorCode::SUCCESS;
}

namespace common::utils::filesystem::internal {

using namespace common::constants::filesystem;

void ConvertGenericCategory(const std::error_code& ec)
{
    static const std::unordered_map<int, ErrorCode> ERR_MAP = {
        {static_cast<int>(std::errc::permission_denied), ErrorCode::PERMISSION_DENIED},
        {static_cast<int>(std::errc::no_such_file_or_directory), ErrorCode::NOT_FOUND},
        {static_cast<int>(std::errc::file_exists), ErrorCode::ALREADY_EXISTS},
        {static_cast<int>(std::errc::directory_not_empty), ErrorCode::DIR_NOT_EMPTY},
        {static_cast<int>(std::errc::is_a_directory), ErrorCode::NOT_FILE},
        {static_cast<int>(std::errc::filename_too_long), ErrorCode::PATH_TOO_LONG},
    };

    auto it = ERR_MAP.find(ec.value());
    SetLastError(it != ERR_MAP.end() ? it->second : ErrorCode::GENERIC_ERROR);
}

void ConvertSystemCategory(const std::error_code& ec)
{
    static const std::unordered_map<int, ErrorCode> ERR_MAP = {
#if COMPILER_MSVC
        {ERROR_DIR_NOT_EMPTY, ErrorCode::DIR_NOT_EMPTY},
        {ERROR_PATH_NOT_FOUND, ErrorCode::NOT_FOUND},
        {ERROR_FILE_EXISTS, ErrorCode::ALREADY_EXISTS}
#endif
#if COMPILER_MINGW
        {EIO, ErrorCode::DIR_NOT_EMPTY}
#endif
    };

    auto it = ERR_MAP.find(ec.value());
    SetLastError(it != ERR_MAP.end() ? it->second : ErrorCode::SYSTEM_ERROR);
}

void SetLastError(ErrorCode code)
{
    g_fileSystemLastError = code;
}

void ConverExceptionToErrorCode(const std::exception& ex)
{
    try {
        std::throw_with_nested(ex);
    } catch (const std::filesystem::filesystem_error& fse) {
        DEBUG_LOG_WARN("File system error: %s (code: %d)", fse.what(), fse.code().value());
        ConvertSysEcToErrorCode(fse.code());
        return;
    } catch (const std::system_error& se) {
        DEBUG_LOG_WARN("System error: %s (code: %d)", se.what(), se.code().value());
        ConvertSysEcToErrorCode(se.code());
        return;
    } catch (const std::exception& other) {
        DEBUG_LOG_EXCEPTION(other, "Non-filesystem exception");
        SetLastError(ErrorCode::SYSTEM_ERROR);
        return;
    }
}

void ConvertSysEcToErrorCode(const std::error_code& ec)
{
    if (!ec) {
        SetLastError(ErrorCode::SUCCESS);
        return;
    }
    // 详细日志便于调试
    DEBUG_LOG_WARN(
        "Convert error: %s (category: %s, value: %d)", ec.message().c_str(), ec.category().name(), ec.value());

    if (ec.category() == std::system_category()) {
        ConvertSystemCategory(ec);
    } else if (ec.category() == std::generic_category()) {
        ConvertGenericCategory(ec);
    } else {
        DEBUG_LOG_FATAL("Unkown");
    }
}

ErrorCode GetLastErrorInternal()
{
    return g_fileSystemLastError;
}

}  // namespace common::utils::filesystem::internal
