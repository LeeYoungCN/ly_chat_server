#include "common/debug/debug_log.h"

#include <cstdarg>

#include "debug_logger_inner.h"

using namespace common::debug;

extern "C" {
void common_debug_log(const char* file, int line, const char* func, DebugLevel level,
                      const char* format, ...)
{
    if (!DebugLoggerInner::instance().should_log(level)) {
        return;
    }

    va_list args;
    va_start(args, format);
    DebugLoggerInner::instance().log_va(file, line, func, level, format, args);
    va_end(args);
}

void set_debug_log_level(DebugLevel level)
{
    DebugLoggerInner::instance().set_debug_log_level(level);
}
}
