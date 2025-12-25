#include "common/utils/error_code_utils.h"

#include "common/common_error_code.h"
#include "common/types/error_code_types.h"

namespace {
thread_local volatile common::types::error_code::ErrorCode g_lastError = ERR_COMM_SUCCESS;
}

namespace common::utils::error_code {
void SetLastError(common::types::error_code::ErrorCode errcode)
{
    g_lastError = errcode;
}

common::types::error_code::ErrorCode GetLastError()
{
    return g_lastError;
}

const char* GetLastErrorStr()
{
    return common::error_code::GetErrorMsg(g_lastError);
}

}  // namespace common::utils::error_code
