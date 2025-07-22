#include <unistd.h>

#include <string>

#include "common/constants.h"
#include "common/debug/log.h"
#include "utils/filesystem_utils.h"

namespace utils {
namespace fs = std::filesystem;

std::string GetProcessPath()
{
    char path[common::MAX_PATH_LEN] = {'\0'};
    auto length = readlink("/proc/self/exe", path, common::MAX_PATH_LEN - 1);
    if (length == -1) {
        COMMON_LOG_ERR("[Linux]Failed to get process path.");
        return "";
    }
    path[length] = '\0';
    return path;
}
}  // namespace utils
