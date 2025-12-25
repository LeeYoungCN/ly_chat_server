#pragma once
#ifndef COMMON_UTILS_ERROR_CODE_UTILS_H
#define COMMON_UTILS_ERROR_CODE_UTILS_H

#include "common/types/error_code_types.h"

namespace common::utils::error_code {
void SetLastError(common::types::error_code::ErrorCode errcode);
common::types::error_code::ErrorCode GetLastError();
const char* GetLastErrorStr();
}
#endif // COMMON_UTILS_ERROR_CODE_UTILS_H
