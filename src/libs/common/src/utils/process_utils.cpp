/**
 * @file process_utils.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-02-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#include "common/utils/process_utils.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <unistd.h>  // Linux的readlink函数
#elif PLATFORM_MACOS
#include <mach-o/dyld.h>  // macOS的_NSGetExecutablePath
#include <unistd.h>
#endif

#include "common/common_error_code.h"
#include "common/compiler/macros.h"
#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_logger.h"
#include "common/types/process_types.h"
#include "common/utils/error_code_utils.h"
#include "common/utils/filesystem_utils.h"

namespace common::process {
using namespace common::filesystem;

std::string GetProcessPath()
{
    char path[MAX_PATH_STD] = {'\0'};
#if PLATFORM_WINDOWS
    DWORD length = GetModuleFileNameA(nullptr, path, MAX_PATH_STD);
    if (length == 0 || length >= MAX_PATH_STD) {
        set_thread_last_err(ERR_COMM_SYSTEM_ERROR);
        DEBUG_LOGGER_ERR("[FAILED][WIN32] Get process path, length: {}", length);
        length = 0;
    }
#elif PLATFORM_LINUX
    auto length = readlink("/proc/self/exe", path, MAX_PATH_STD - 1);
    if (length == -1) {
        set_thread_last_err(ERR_COMM_SYSTEM_ERROR);
        DEBUG_LOGGER_ERR("[FAILED][Linux]Failed to get process path.");
        length = 0;
    }
    path[length] = '\0';
#elif PLATFORM_MACOS
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) != 0) {
        set_thread_last_err(ERR_COMM_SYSTEM_ERROR);
        DEBUG_LOGGER_ERR("[FAILED][Macos]Failed to get process path.");
    }
#else
#error "Unsupport system."
#endif
    set_thread_last_err(ERR_COMM_SUCCESS);
    DEBUG_LOGGER_DBG("[SUCCESS] Get process path: {}", path);
    return path;
}

std::string GetProcessDirectory()
{
    set_thread_last_err(ERR_COMM_SUCCESS);
    return GetDirectory(GetProcessPath());
}

std::string GetProcessFileName()
{
    set_thread_last_err(ERR_COMM_SUCCESS);
    return GetFileName(GetProcessPath());
}

ProcessId GetCurrProcessId()
{
    ProcessId pid = 0;
#if PLATFORM_WINDOWS
    pid = GetCurrentProcessId();
#else
    pid = getpid();
#endif
    return pid;
}
}  // namespace common::process
