#ifndef LOGGING_C_COMMON_C_H
#define LOGGING_C_COMMON_C_H

#include <memory>
#include <vector>

#include "logging/c/logging_c.h"
#include "logging/details/task_pool.h"
#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/logger.h"
#include "logging/sinks/sink.h"

struct LoggerSt {
    std::shared_ptr<logging::Logger> ptr;

    explicit LoggerSt(std::shared_ptr<logging::Logger> ptr) : ptr(std::move(ptr)) {}
    explicit LoggerSt(logging::Logger *ptr) : ptr(ptr) {}
};

struct SinkSt {
    std::shared_ptr<logging::Sink> ptr;

    explicit SinkSt(std::shared_ptr<logging::Sink> sink) : ptr(std::move(sink)) {}
    explicit SinkSt(logging::Sink *sink) : ptr(sink) {}
};

struct FormatterSt {
    std::unique_ptr<logging::Formatter> ptr;

    explicit FormatterSt(std::unique_ptr<logging::Formatter> formatter) : ptr(std::move(formatter))
    {
    }
    explicit FormatterSt(logging::Formatter *formatter) : ptr(formatter) {}
};

struct TaskPoolSt {
    std::shared_ptr<logging::details::TaskPool> ptr;

    explicit TaskPoolSt(std::shared_ptr<logging::details::TaskPool> taskPool)
        : ptr(std::move(taskPool))
    {
    }
    explicit TaskPoolSt(logging::details::TaskPool *taskPool) : ptr(taskPool) {}
};

namespace logging::c {
LogLevel logging_to_log_level(LoggingLevel level);
LoggingLevel log_to_logging_level(LogLevel level);
std::vector<std::shared_ptr<Sink>> sink_ptr_vector(const SinkSt *const sinks[], uint32_t sinkCnt);

}  // namespace logging::c
#endif  // LOGGING_C_COMMON_C_H
