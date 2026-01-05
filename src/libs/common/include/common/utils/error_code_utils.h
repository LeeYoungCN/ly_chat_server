#pragma once
#ifndef COMMON_UTILS_ERROR_CODE_UTILS_H
#define COMMON_UTILS_ERROR_CODE_UTILS_H

#include "common/types/error_code_types.h"

extern "C" {
void set_thread_last_err(ErrorCode errcode);
ErrorCode get_thread_last_err();
const char* get_thread_last_err_msg();
}
#endif  // COMMON_UTILS_ERROR_CODE_UTILS_H
