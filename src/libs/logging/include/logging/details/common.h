#pragma once
#ifndef LOGGING_DETAILS_COMMON_H
#define LOGGING_DETAILS_COMMON_H

#include <cstdint>

#define LOG_SRC_LOCAL logging::details::LogSource(__FILE__, __LINE__, __FUNCTION__)
#define LOG_SRC_EMPTY logging::details::LogSource()

namespace logging {
constexpr const char* DEFAULT_PATTERN = "[%d][%L][%s:%#]: %v";
constexpr const char* DEFAULT_TIME_PATTERN = "%Y-%m-%d %H:%M:%S.%3f";
constexpr const char* DEFAULT_LOGGER_NAME = "__default_logger__";

constexpr uint32_t THREAD_POOL_DEFAULT_CAPACITY = 4096;
constexpr uint32_t THREAD_POOL_DEFAULT_THREAD_CNT = 1;
constexpr uint32_t LOG_CONTENT_DEFAULT_LEN = 256;
}  // namespace logging

#endif  // LOGGING_DETAILS_COMMON_H
