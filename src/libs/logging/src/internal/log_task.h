#pragma once
#ifndef LOGGING_INTERNAL_LOG_TASK_H
#define LOGGING_INTERNAL_LOG_TASK_H

#include <memory>
#include <utility>

#include "logging/log_msg.h"

namespace logging {
class AsyncLoggerImpl;

enum class TaskType {
    LOG,
    FLUSH,
    SHUTDOWN,
};

struct LogTask {
    TaskType type = TaskType::SHUTDOWN;
    LogMsg logMsg;
    std::shared_ptr<AsyncLoggerImpl> logger;

    LogTask() = default;
    explicit LogTask(TaskType type) : type(type) {}

    LogTask(TaskType type, const std::shared_ptr<AsyncLoggerImpl>& logger)
        : type(type), logger(logger)
    {
    }

    LogTask(TaskType type, const std::shared_ptr<AsyncLoggerImpl>& logger, LogMsg logMsg)
        : type(type), logMsg(std::move(logMsg)), logger(logger)
    {
    }
};

}  // namespace logging

#endif  // LOGGING_INTERNAL_LOG_TASK_H
