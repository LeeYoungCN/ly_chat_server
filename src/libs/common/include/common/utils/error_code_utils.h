#pragma once
#ifndef COMMON_UTILS_ERROR_CODE_UTILS_H
#define COMMON_UTILS_ERROR_CODE_UTILS_H

#include "common/types/error_code_types.h"

extern "C" {
void SetLastErrcode(ErrorCode errcode);
ErrorCode GetLastErrcode();
const char* GetLastErrorStr();
}
#endif  // COMMON_UTILS_ERROR_CODE_UTILS_H
