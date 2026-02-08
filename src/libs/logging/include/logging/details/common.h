#pragma once

#ifndef LOGGING_DETAILS_COMMON_H
#define LOGGING_DETAILS_COMMON_H

#ifdef __cplusplus

#include <cstdint>
#include <string_view>

namespace logging {
constexpr std::string_view LOGGER_DEFAULT_NAME = "__default_logger__";

constexpr std::string_view FORMATTER_DEFAULT_PATTERN = "[%d][%L][%s:%#]: %v";
constexpr std::string_view FORMATTER_DEFAULT_TIME_PATTERN = "%Y-%m-%d %H:%M:%S.%3f";

constexpr uint32_t THREAD_POOL_DEFAULT_CAPACITY = 4096;
constexpr uint32_t THREAD_POOL_DEFAULT_THREAD_CNT = 1;
}  // namespace logging
#else
#define LOGGER_DEFAULT_NAME            "__default_logger__"
#define FORMATTER_DEFAULT_PATTERN      "[%d][%L][%s:%#]: %v"
#define FORMATTER_DEFAULT_TIME_PATTERN "%Y-%m-%d %H:%M:%S.%3f"
#define THREAD_POOL_DEFAULT_CAPACITY   4096
#define THREAD_POOL_DEFAULT_THREAD_CNT 1
#endif

#endif  // LOGGING_DETAILS_COMMON_H
