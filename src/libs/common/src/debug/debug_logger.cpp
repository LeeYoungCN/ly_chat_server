#include "common/debug/debug_logger.h"

#include "debug_logger_inner.h"

namespace common::debug {
void set_common_debug_logger_level(DebugLevel level)
{
    DebugLoggerInner::instance().set_debug_log_level(level);
}

void common_debug_logger_log(const char* file, int line, const char* func, DebugLevel level,
                             const std::string& message)
{
    DebugLoggerInner::instance().log(file, line, func, level, message);
}
}  // namespace common::debug
