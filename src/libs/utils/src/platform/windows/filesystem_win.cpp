#include <windows.h>

#include <string>

#include "common/constants.h"
#include "common/debug/log.h"
#include "utils/filesystem_utils.h"

namespace utils {

std::string GetProcessPath()
{
    char path[common::MAX_PATH_LEN] = {'\0'};

    DWORD length = GetModuleFileNameA(nullptr, path, common::MAX_PATH_LEN);
    if (length == 0 || length >= common::MAX_PATH_LEN) {
        COMMON_LOG_ERR("[WIN32]Failed to get process path, length: {}", length);
        return "";
    }

    return path;
}
}  // namespace utils
