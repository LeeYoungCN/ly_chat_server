#ifndef LOGGING_C_COMMON_C_H
#define LOGGING_C_COMMON_C_H

#include <memory>
#include <vector>

#include "internal/task_pool.h"
#include "logging/c/logging_c.h"
#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/loggers/sync_logger.h"
#include "logging/sinks/sink.h"
using namespace origin::logging;
struct LoggerSt {
    std::shared_ptr<SyncLogger> ptr;

    explicit LoggerSt(std::shared_ptr<SyncLogger> ptr) : ptr(std::move(ptr)) {}
    explicit LoggerSt(SyncLogger *ptr) : ptr(ptr) {}
};

struct SinkSt {
    std::shared_ptr<Sink> ptr;

    explicit SinkSt(std::shared_ptr<Sink> sink) : ptr(std::move(sink)) {}
    explicit SinkSt(origin::logging::Sink *sink) : ptr(sink) {}
};

struct FormatterSt {
    std::unique_ptr<Formatter> ptr;

    explicit FormatterSt(std::unique_ptr<Formatter> formatter) : ptr(std::move(formatter)) {}
    explicit FormatterSt(origin::logging::Formatter *formatter) : ptr(formatter) {}
};

struct TaskPoolSt {
    std::shared_ptr<TaskPool> ptr;

    explicit TaskPoolSt(std::shared_ptr<TaskPool> taskPool) : ptr(std::move(taskPool)) {}
    explicit TaskPoolSt(origin::logging::TaskPool *taskPool) : ptr(taskPool) {}
};

namespace origin::logging::c {
LogLevel logging_to_log_level(LoggingLevel level);
LoggingLevel log_to_logging_level(LogLevel level);
std::vector<std::shared_ptr<Sink>> sink_ptr_vector(const SinkSt *const sinks[], uint32_t sinkCnt);

}  // namespace origin::logging::c
#endif  // LOGGING_C_COMMON_C_H
