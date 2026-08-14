#ifndef LOGGING_INTERNAL_DETAILS_COMMON_H
#define LOGGING_INTERNAL_DETAILS_COMMON_H

#include <cstdint>
#include <string>

namespace logging::details {
std::string get_default_log_file(std::string_view suffix = "log");

bool delete_file(std::string_view file, uint32_t maxRetry = 3, uint32_t sleepMs = 100);

bool rename_file(std::string_view src, std::string_view dest, bool overwrite = true,
                 uint32_t maxRetry = 3, uint32_t sleepMs = 100);

}  // namespace logging::internal

#endif  // LOGGING_INTERNAL_DETAILS_COMMON_H
