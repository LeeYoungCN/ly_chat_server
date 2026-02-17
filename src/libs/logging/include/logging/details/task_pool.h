#pragma once
#ifndef LOGGING_DETAILS_LOG_THREAD_POOL_H
#define LOGGING_DETAILS_LOG_THREAD_POOL_H

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include "common/compiler/macros.h"
#include "common/container/concurrent_blocking_queue.hpp"
#include "logging/details/log_msg.h"
#include "logging/details/log_task.h"

namespace logging {
class AsyncLogger;

namespace details {

class COMMON_API TaskPool {
public:
    TaskPool();
    ~TaskPool();
    explicit TaskPool(uint32_t capacity);
    TaskPool(uint32_t capacity, uint32_t threadCnt);

    void log(const std::shared_ptr<AsyncLogger>& logger, const LogMsg& logMsg);
    void flush(const std::shared_ptr<AsyncLogger>& logger);

protected:
    void worker_loop(uint32_t idx);

private:
    std::unique_ptr<common::container::ConcurrentBlockingQueue<LogTask>> _logBuffer;
    uint32_t _threadCnt = 0;
    std::unique_ptr<std::vector<std::thread>> _threadPool;
};
}  // namespace details
}  // namespace logging

#endif  // LOGGING_DETAILS_LOG_THREAD_POOL_H
