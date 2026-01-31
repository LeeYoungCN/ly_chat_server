#include "common/debug/debug_log.h"

#include <cstdarg>

#include "common/debug/debug_logger.h"

using namespace common::debug;

extern "C" {
void common_debug_log_c(DebugLevel level, const char* file, int line, const char* func, const char* format, ...)
{
    if (!DebugLogger::instance().should_log(level)) {
        return;
    }

    va_list args;
    va_start(args, format);
    DebugLogger::instance().log_va(level, file, line, func, format, args);
    va_end(args);
}

void set_debug_log_level(DebugLevel level)
{
    DebugLogger::instance().set_debug_log_level(level);
}
}
