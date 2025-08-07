
#include "internal/common/utils/date_time_utils_internal.h"

#include "common/constants/date_time_constants.h"

namespace {
thread_local volatile common::constants::date_time::ErrorCode g_timeLastError =
    common::constants::date_time::ErrorCode::SUCCESS;
}

namespace common::utils::date_time_utils::internal {

using namespace common::constants::date_time;

void SetLastError(ErrorCode code)
{
    g_timeLastError = code;
}

ErrorCode GetLastErrorInternal()
{
    return g_timeLastError;
}

}  // namespace common::utils::date_time_utils::internal
