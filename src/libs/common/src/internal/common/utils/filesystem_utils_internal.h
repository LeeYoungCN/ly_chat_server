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
#ifndef COMMON_UTILS_FILESYSTEM_UTILS_INTERNAL_H
#define COMMON_UTILS_FILESYSTEM_UTILS_INTERNAL_H

#include <system_error>

#include "common/constants/filesystem_constants.h"

namespace common::utils::filesystem::internal {

void SetLastError(constants::filesystem::ErrorCode code);

void ConvertSysEcToErrorCode(const std::error_code& ec);

void ConverExceptionToErrorCode(const std::exception& ex);

constants::filesystem::ErrorCode GetLastErrorInternal();

}  // namespace common::utils::filesystem::internal

#endif  // COMMON_UTILS_FILESYSTEM_UTILS_INTERNAL_H
