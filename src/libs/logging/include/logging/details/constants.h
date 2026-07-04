#pragma once
#ifndef LOGGING_DETAILS_CONSTANTS_H
#define LOGGING_DETAILS_CONSTANTS_H

#include <string_view>

namespace logging::details {
[[maybe_unused]] constexpr std::string_view ROOT_LOGGER_NAME = "__root_logger__";

[[maybe_unused]] constexpr std::string_view FORMATTER_DEFAULT_PATTERN = "[%d][%l][%s:%#]: %v";
[[maybe_unused]] constexpr std::string_view FORMATTER_DEFAULT_TIME_PATTERN =
    "%Y-%m-%d %H:%M:%S.%3f";

}  // namespace logging::details

#endif  // LOGGING_DETAILS_CONSTANTS_H
