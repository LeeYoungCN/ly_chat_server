#pragma once
#ifndef LOGGING_DETAILS_COMMON_H
#define LOGGING_DETAILS_COMMON_H

#include <cstdint>
#include <string_view>

namespace logging::details {
[[maybe_unused]] constexpr std::string_view ROOT_LOGGER_NAME = "__root_logger__";

[[maybe_unused]] constexpr std::string_view FORMATTER_DEFAULT_PATTERN = "[%d][%L][%s:%#]: %v";
[[maybe_unused]] constexpr std::string_view FORMATTER_DEFAULT_TIME_PATTERN =
    "%Y-%m-%d %H:%M:%S.%3f";

[[maybe_unused]] constexpr uint32_t THREAD_POOL_DEFAULT_CAPACITY = 4096;
[[maybe_unused]] constexpr uint32_t THREAD_POOL_DEFAULT_THREAD_CNT = 1;
}  // namespace logging::details

#define LOG_SRC_LOCAL                  logging::details::LogSource(__FILE__, __LINE__, __FUNCTION__)
#define LOG_SRC_EMPTY                  logging::details::LogSource()
#define LOG_SRC_INST(file, line, func) logging::details::LogSource(file, line, func)

#endif  // LOGGING_DETAILS_COMMON_H
