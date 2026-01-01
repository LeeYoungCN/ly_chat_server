#include "common/utils/error_code_utils.h"

#include "common/common_error_code.h"
#include "common/types/error_code_types.h"

namespace {
thread_local volatile ErrorCode g_lastError = ERR_COMM_SUCCESS;
}

extern "C" {
void SetLastErrcode(ErrorCode errcode)
{
    g_lastError = errcode;
}

ErrorCode GetLastErrcode()
{
    return g_lastError;
}

const char* GetLastErrorStr()
{
    return GetCommErrorMsg(g_lastError);
}

}  // namespace common::utils::error_code
