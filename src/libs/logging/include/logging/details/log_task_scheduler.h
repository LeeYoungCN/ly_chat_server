#pragma once
#ifndef LOGGING_DETAILS_LOG_TASK_SCHEDULER_H
#define LOGGING_DETAILS_LOG_TASK_SCHEDULER_H

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include "common/container/concurrent_blocking_queue.hpp"
#include "logging/details/log_msg.h"
#include "logging/details/log_task.h"

namespace logging {
class AsyncLogger;

namespace details {

class LogTaskScheduler {
public:
    LogTaskScheduler();
    ~LogTaskScheduler();
    LogTaskScheduler(uint32_t threadCnt, uint32_t bufferCapacity);

    void log(std::shared_ptr<AsyncLogger> logger, const LogMsg& logMsg);
    void flush(std::shared_ptr<AsyncLogger> logger);

protected:
    void worker_loop();

private:
    common::container::ConcurrentBlockingQueue<LogTask> _logBuffer;
    std::vector<std::thread> _threadPool;
};
}  // namespace details
}  // namespace logging

#endif  // LOGGING_DETAILS_LOG_TASK_SCHEDULER_H
