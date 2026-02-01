#ifndef LOGGING_DETAILS_LOG_TASK_SCHEDULER_H
#define LOGGING_DETAILS_LOG_TASK_SCHEDULER_H

#include <cstdint>
#include <memory>
#include <queue>
#include <thread>
#include <utility>
#include <vector>

#include "logging/async_logger.h"
#include "logging/details/log_msg.h"

namespace logging {
class AsyncLogger;
namespace details {
using logger_ptr = std::shared_ptr<AsyncLogger>;
enum class TaskType {
    LOG,
    FLUSH,
    SHUTDOWN,
};

struct LogTask {
    TaskType type = TaskType::SHUTDOWN;
    LogMsg logMsg;
    logger_ptr logger;

    explicit LogTask(TaskType type) : LogTask(type, nullptr, LogMsg()) {}

    LogTask(TaskType type, logger_ptr logger) : LogTask(type, std::move(logger), LogMsg()) {}

    LogTask(TaskType type, logger_ptr logger, LogMsg logMsg)
        : type(type), logMsg(std::move(logMsg)), logger(std::move(logger))
    {
    }
};

class LogTaskScheduler {
public:
    LogTaskScheduler();
    ~LogTaskScheduler();
    LogTaskScheduler(uint32_t threadCnt, uint32_t bufferCapacity);

    void log(logger_ptr&& logger, LogMsg&& logMsg);
    void flush(logger_ptr&& logger);

protected:
    void worker_loop();

private:
    std::vector<std::thread> _threadPool;
    // TODO: 使用无锁队列
    std::queue<LogTask> _logBuffer;
    std::mutex _bufferMtx;
    std::condition_variable _bufferCv;

    uint32_t _threadCnt = 1;
    uint32_t _bufferCapacity = 1024;
};
}  // namespace details
}  // namespace logging

#endif  // LOGGING_DETAILS_LOG_TASK_SCHEDULER_H
