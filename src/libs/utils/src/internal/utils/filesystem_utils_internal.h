/**
 * @file filesystem_utils_internal.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef UTILS_FILESYSTEM_UTILS_INTERNAL_H
#define UTILS_FILESYSTEM_UTILS_INTERNAL_H

#include <system_error>

#include "common/types/error_code_types.h"

namespace utils::filesystem::internal {

ErrorCode ConvertSysEcToErrorCode(const std::error_code& ec);

ErrorCode ConvertExceptionToErrorCode(const std::exception& ex);

}  // namespace utils::filesystem::internal

#endif  // UTILS_FILESYSTEM_UTILS_INTERNAL_H
