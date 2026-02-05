#pragma once
#ifndef LOGGING_DETAILS_LOG_TASK_H
#define LOGGING_DETAILS_LOG_TASK_H

#include <memory>
#include <utility>

#include "logging/details/log_msg.h"

namespace logging {
class AsyncLogger;

namespace details {
enum class TaskType {
    LOG,
    FLUSH,
    SHUTDOWN,
};

struct LogTask {
    TaskType type = TaskType::SHUTDOWN;
    LogMsg logMsg;
    std::shared_ptr<AsyncLogger> logger;

    LogTask() = default;
    explicit LogTask(TaskType type) : type(type) {}

    LogTask(TaskType type, std::shared_ptr<AsyncLogger> logger) : type(type), logger(std::move(logger)) {}

    LogTask(TaskType type, std::shared_ptr<AsyncLogger> logger, LogMsg logMsg)
        : type(type), logMsg(std::move(logMsg)), logger(std::move(logger))
    {
    }
};
}  // namespace details
}  // namespace logging

#endif  // LOGGING_DETAILS_LOG_TASK_H
