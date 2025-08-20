/**
 * @file log_source.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-08-08
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef LOGGING_DETAILS_LOG_SOURCE_H
#define LOGGING_DETAILS_LOG_SOURCE_H

#include <cstdint>

namespace logging::details {

struct LogSource {
    const char *file{nullptr};
    int32_t line{-1};
    const char *func{nullptr};

    [[nodiscard]] bool isEmpty() const;
    LogSource() = default;
    LogSource(const char *file, int32_t line, const char *func);
};

#define LOG_SOURCE                       \
    logging::details::LogSource          \
    {                                    \
        __FILE__, __LINE__, __FUNCTION__ \
    }

}  // namespace logging::details

#endif  // LOGGING_DETAILS_LOG_SOURCE_H
