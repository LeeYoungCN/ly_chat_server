#ifndef LOGGING_DETAILS_LOG_TASK_SCHEDULER_H
#define LOGGING_DETAILS_LOG_TASK_SCHEDULER_H

#include <cstdint>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#include "common/container/concurrent_blocking_queue.hpp"
#include "logging/async_logger.h"
#include "logging/details/log_msg.h"

namespace logging {
class AsyncLogger;
namespace details {
using LoggerPtr = std::shared_ptr<AsyncLogger>;
enum class TaskType {
    LOG,
    FLUSH,
    SHUTDOWN,
};

struct LogTask {
    TaskType type = TaskType::SHUTDOWN;
    LogMsg logMsg;
    LoggerPtr logger;

    LogTask() = default;
    explicit LogTask(TaskType type) : LogTask(type, nullptr, LogMsg()) {}

    LogTask(TaskType type, LoggerPtr logger) : LogTask(type, std::move(logger), LogMsg()) {}

    LogTask(TaskType type, LoggerPtr logger, LogMsg logMsg)
        : type(type), logMsg(std::move(logMsg)), logger(std::move(logger))
    {
    }
};

class LogTaskScheduler {
public:
    LogTaskScheduler();
    ~LogTaskScheduler();
    LogTaskScheduler(uint32_t threadCnt, uint32_t bufferCapacity);

    void log(LoggerPtr&& logger, LogMsg&& logMsg);
    void flush(LoggerPtr&& logger);

protected:
    void worker_loop();

private:
    common::container::ConcurrentBlockingQueue<LogTask> _logBuffer;
    std::vector<std::thread> _threadPool;
};
}  // namespace details
}  // namespace logging

#endif  // LOGGING_DETAILS_LOG_TASK_SCHEDULER_H
