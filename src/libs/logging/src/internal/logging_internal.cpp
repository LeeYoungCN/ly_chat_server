#include "logging_internal.h"

#include "common/debug/debug_logger.h"
#include "utils/date_time_utils.h"
#include "utils/filesystem_utils.h"
#include "utils/process_utils.h"
#include "utils/thread_utils.h"

namespace logging::internal {
using namespace utils::filesystem;
using namespace utils::process;

std::string get_default_log_file(std::string_view suffix)
{
    std::string process = get_proc_path();
    return join_paths(
        {get_directory(process), "logs", get_file_name_stem(process) + "." + std::string(suffix)});
}

bool delete_file(std::string_view path, uint32_t maxRetry, uint32_t sleepMs)
{
    for (uint32_t i = 0; i < maxRetry; i++) {
        if (utils::filesystem::delete_file(path)) {
            DEBUG_LOGGER_DBG("Delete file success. file: \"{}\".", path);
            return true;
        }
        utils::date_time::sleep_ms(sleepMs);
    }

    DEBUG_LOGGER_ERR(
        "Delete file failed. file: \"{}\", msg: \"{}\".", path, get_thread_last_err_msg());
    return false;
}

bool rename_file(std::string_view src, std::string_view dest, bool overwrite, uint32_t maxRetry,
                 uint32_t sleepMs)
{
    for (uint32_t i = 0; i < maxRetry; i++) {
        if (utils::filesystem::rename_file(src, dest, overwrite)) {
            DEBUG_LOGGER_DBG("Rename file success. file: \"{}\".", src);
            return true;
        }
        utils::date_time::sleep_ms(sleepMs);
    }

    DEBUG_LOGGER_ERR(
        "Rename file failed. file: \"{}\", msg: \"{}\".", src, get_thread_last_err_msg());
    return false;
}

}  // namespace logging::internal
