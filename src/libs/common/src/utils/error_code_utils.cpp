#include "common/utils/error_code_utils.h"

#include "common/common_error_code.h"
#include "common/types/error_code_types.h"

namespace {
thread_local volatile ErrorCode g_lastError = ERR_COMM_SUCCESS;
}

extern "C" {
void set_thread_last_err(ErrorCode errcode)
{
    g_lastError = errcode;
}

ErrorCode get_thread_last_err()
{
    return g_lastError;
}

const char* get_thread_last_err_msg()
{
    return get_comm_err_msg(g_lastError);
}

}  // namespace common::utils::error_code
