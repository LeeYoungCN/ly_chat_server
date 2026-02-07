#ifndef LOGGING_C_LOGGING_LEVEL_C_H
#define LOGGING_C_LOGGING_LEVEL_C_H

#include "logging/log_level.h"
#include "logging/logging_c.h"

namespace logging::c {
LogLevel logging_to_log_level(LoggingLevel level);
LoggingLevel log_to_logging_level(LogLevel level);
}  // namespace logging::c
#endif  // LOGGING_C_LOGGING_LEVEL_C_H
