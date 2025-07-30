#include "internal/common/utils/filesystem_utils_internal.h"

#include <memory>
#include <system_error>

#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_log.h"
#include "common/types/filesystem_types.h"
#include "common/utils/filesystem_utils.h"
#include "internal/common/utils/filesystem_utils_internal.h"

namespace common::utils::filesystem::internal {

using namespace common::constants::filesystem;
using namespace common::types::filesystem;

void ConvertSysEcToErrorCode(const std::error_code& ec)
{
    if (!ec) {
        SetLastError(ErrorCode::SUCCESS);
        return;
    }
    COMMON_LOG_ERR("Convert err: %s", ec.message().c_str());
    switch (ec.value()) {
        case static_cast<int>(std::errc::permission_denied):
            SetLastError(ErrorCode::PERMISSION_DENIED);
            break;
        case static_cast<int>(std::errc::no_such_file_or_directory):
            SetLastError(ErrorCode::NOT_FOUND);
            break;
        case static_cast<int>(std::errc::file_exists):
            SetLastError(ErrorCode::ALREADY_EXISTS);
            break;
        case static_cast<int>(std::errc::invalid_argument):
            SetLastError(ErrorCode::PATH_INVALID);
            break;
        case static_cast<int>(std::errc::directory_not_empty):
            SetLastError(ErrorCode::DIR_NOT_EMPTY);
            break;
        case static_cast<int>(std::errc::is_a_directory):
            SetLastError(ErrorCode::NOT_FILE);
            break;
        case static_cast<int>(std::errc::filename_too_long):
            SetLastError(ErrorCode::PATH_TOO_LONG);
            break;
        default:
            SetLastError(ErrorCode::SYSTEM_ERROR);
            break;
    }
}
}  // namespace common::utils::filesystem::internal
