#include "logging_internal.h"

#include "utils/filesystem_utils.h"
#include "utils/process_utils.h"

namespace logging::internal {
using namespace utils::filesystem;
using namespace utils::process;

std::string get_default_log_file(std::string_view suffix)
{
    std::string process = get_proc_path();
    return join_paths(
        {get_directory(process), "logs", get_file_name_stem(process) + "." + std::string(suffix)});
}

}  // namespace logging::internal
