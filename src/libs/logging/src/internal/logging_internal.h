#ifndef LOGGING_INTERNAL_LOGGING_INTERNAL_H
#define LOGGING_INTERNAL_LOGGING_INTERNAL_H

#include <string>
namespace logging::internal {
std::string get_default_log_file(std::string_view suffix = "log");
}

#endif // LOGGING_INTERNAL_LOGGING_INTERNAL_H