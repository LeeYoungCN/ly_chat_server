#pragma once
#ifndef LOGGING_DETAILS_COMMON_H
#define LOGGING_DETAILS_COMMON_H

#include <cstdint>

static const char* LOGGER_DEFAULT_NAME = "__default_logger__";

static const char* FORMATTER_DEFAULT_PATTERN = "[%d][%L][%s:%#]: %v";
static const char* FORMATTER_DEFAULT_TIME_PATTERN = "%Y-%m-%d %H:%M:%S.%3f";

static uint32_t THREAD_POOL_DEFAULT_CAPACITY = 4096;
static uint32_t THREAD_POOL_DEFAULT_THREAD_CNT = 1;

#endif  // LOGGING_DETAILS_COMMON_H
