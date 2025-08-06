/**
 * @file date_time_internal.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-07-29
 *
 * @copyright Copyright (c) 2025
 *
 */
#pragma once
#ifndef COMMON_UTILS_DATA_TIME_UTILS_INTERNAL_H
#define COMMON_UTILS_DATA_TIME_UTILS_INTERNAL_H

#include "common/constants/date_time_constants.h"

namespace common::utils::date_time_utils::internal {

void SetLastError(common::constants::date_time::ErrorCode code);

common::constants::date_time::ErrorCode GetLastErrorInternal();
}  // namespace common::utils::date_time_utils::internal

#endif  // COMMON_UTILS_DATA_TIME_UTILS_INTERNAL_H
