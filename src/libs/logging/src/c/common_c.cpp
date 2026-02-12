#include "c/common_c.h"

#include <cstdint>
#include <memory>
#include <vector>

#include "logging/c/logging_c.h"
#include "logging/log_level.h"

namespace logging::c {
LogLevel logging_to_log_level(LoggingLevel level)
{
    switch (level) {
        case LOGGING_LEVEL_DEBUG:
            return LogLevel::DEBUG;
        case LOGGING_LEVEL_INFO:
            return LogLevel::INFO;
        case LOGGING_LEVEL_WARN:
            return LogLevel::WARN;
        case LOGGING_LEVEL_ERROR:
            return LogLevel::ERROR;
        case LOGGING_LEVEL_FATAL:
            return LogLevel::FATAL;
        case LOGGING_LEVEL_OFF:
        default:
            return LogLevel::OFF;
    }
}

LoggingLevel log_to_logging_level(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return LOGGING_LEVEL_DEBUG;
        case LogLevel::INFO:
            return LOGGING_LEVEL_INFO;
        case LogLevel::WARN:
            return LOGGING_LEVEL_WARN;
        case LogLevel::ERROR:
            return LOGGING_LEVEL_ERROR;
        case LogLevel::FATAL:
            return LOGGING_LEVEL_FATAL;
        case LogLevel::OFF:
        default:
            return LOGGING_LEVEL_OFF;
    }
}

std::vector<std::shared_ptr<Sink>> sink_ptr_vector(const SinkSt* const sinks[], uint32_t sinkCnt)
{
    std::vector<std::shared_ptr<Sink>> sinkPtrs;
    sinkPtrs.reserve(sinkCnt);
    for (uint32_t i = 0; i < sinkCnt; ++i) {
        if (sinks[i]->ptr != nullptr) {
            sinkPtrs.emplace_back(sinks[i]->ptr);
        }
    }
    return sinkPtrs;
}
}  // namespace logging::c
