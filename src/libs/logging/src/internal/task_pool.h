#pragma once
#ifndef LOGGING_INTERNAL_TASK_POOL_H
#define LOGGING_INTERNAL_TASK_POOL_H

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "common/container/concurrent_blocking_queue.hpp"
#include "internal/log_task.h"
#include "logging/log_msg.h"

namespace logging {
class AsyncLoggerImpl;
class TaskPool {
public:
    static constexpr uint32_t DEFAULT_CAPACITY = 4096;
    static constexpr uint32_t DEFAULT_THREAD_CNT = 1;

public:
    TaskPool();
    ~TaskPool();
    explicit TaskPool(uint32_t capacity);
    TaskPool(uint32_t capacity, uint32_t threadCnt);

    void log(const std::shared_ptr<AsyncLoggerImpl>& logger, const LogMsg& logMsg);
    void flush(const std::shared_ptr<AsyncLoggerImpl>& logger);
    [[nodiscard]] size_t task_count();

private:
    void start();
    void shutdown();
    void worker_loop(uint32_t idx);

private:
    common::container::ConcurrentBlockingQueue<LogTask> _buffer;
    uint32_t _threadCnt = 0;
    std::vector<std::thread> _threadPool;
    std::mutex _threadPoolMtx;
    std::atomic<bool> _isThreadRunning{false};
};
}  // namespace logging

#endif  // LOGGING_INTERNAL_TASK_POOL_H
