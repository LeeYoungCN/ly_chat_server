#include "internal/common/utils/filesystem_utils_internal.h"

#include "common/common_error_code.h"
#include "common/compiler/macros.h"
#include "common/types/error_code_types.h"

#if COMPILER_MSVC
#include <windows.h>
#else
#include <cerrno>
#endif

#include <exception>
#include <filesystem>
#include <system_error>
#include <unordered_map>

#include "common/common_error_code.h"
#include "common/debug/debug_log.h"
#include "common/types/error_code_types.h"
#include "common/utils/error_code_utils.h"
#include "internal/common/utils/filesystem_utils_internal.h"

namespace common::utils::filesystem::internal {

using namespace common::types::error_code;
using namespace common::utils::error_code;
using namespace common::error_code;

void ConvertGenericCategory(const std::error_code& ec)
{
    static const std::unordered_map<int, ErrorCode> ERR_MAP = {
        {static_cast<int>(std::errc::permission_denied), ERR_COMM_PERMISSION_DENIED},
        {static_cast<int>(std::errc::no_such_file_or_directory), ERR_COMM_NOT_FOUND},
        {static_cast<int>(std::errc::file_exists), ERR_COMM_ALREADY_EXISTS},
        {static_cast<int>(std::errc::directory_not_empty), ERR_COMM_DIR_NOT_EMPTY},
        {static_cast<int>(std::errc::is_a_directory), ERR_COMM_IS_A_DIRECTORY},
        {static_cast<int>(std::errc::filename_too_long), ERR_COMM_PATH_TOO_LONG},
        {static_cast<int>(std::errc::invalid_argument), ERR_COMM_PATH_INVALID},
        {static_cast<int>(std::errc::io_error), ERR_COMM_IO_ERROR}};

    auto it = ERR_MAP.find(ec.value());
    SetLastError(it != ERR_MAP.end() ? it->second : ERR_COMM_GENERIC_ERROR);
}

void ConvertSystemCategory(const std::error_code& ec)
{
    static const std::unordered_map<int, ErrorCode> ERR_MAP = {
#if COMPILER_MSVC
        {ERROR_FILE_NOT_FOUND, ERR_COMM_NOT_FOUND},
        {ERROR_DIR_NOT_EMPTY, ERR_COMM_DIR_NOT_EMPTY},
        {ERROR_PATH_NOT_FOUND, ERR_COMM_NOT_FOUND},
        {ERROR_FILE_EXISTS, ERR_COMM_ALREADY_EXISTS},
        {ERROR_SHARING_VIOLATION, ERR_COMM_SHARING_VIOLATION},
        {ERROR_ACCESS_DENIED, ERR_COMM_PERMISSION_DENIED}
#else
        {EIO, ERR_COMM_IO_ERROR}, {EBUSY, ERR_COMM_SHARING_VIOLATION}
#endif
    };

    auto it = ERR_MAP.find(ec.value());
    SetLastError(it != ERR_MAP.end() ? it->second : ERR_COMM_SYSTEM_ERROR);
}

void ConvertExceptionToErrorCode(const std::exception& ex)
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
        SetLastError(ERR_COMM_SYSTEM_ERROR);
        return;
    }
}

void ConvertSysEcToErrorCode(const std::error_code& ec)
{
    if (!ec) {
        SetLastError(ERR_COMM_SUCCESS);
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

}  // namespace common::utils::filesystem::internal
