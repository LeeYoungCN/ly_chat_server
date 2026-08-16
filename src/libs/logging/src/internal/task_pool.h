#pragma once
#ifndef ORIGIN_LOGGING_INTERNAL_TASK_POOL_H
#define ORIGIN_LOGGING_INTERNAL_TASK_POOL_H

#include <cstdint>
#include <memory>
#include <thread>
#include <vector>

#include "common/container/concurrent_blocking_queue.hpp"
#include "internal/log_task.h"
#include "logging/log_msg.h"

namespace origin::logging {
class LoggerImpl;
class TaskPool {
public:
    static constexpr uint32_t DEFAULT_CAPACITY = 4096;
    static constexpr uint32_t DEFAULT_THREAD_CNT = 1;

public:
    TaskPool();
    ~TaskPool();
    explicit TaskPool(uint32_t capacity);
    TaskPool(uint32_t capacity, uint32_t threadCnt);

    void log(const std::shared_ptr<LoggerImpl>& logger, const LogMsg& logMsg);
    void flush(const std::shared_ptr<LoggerImpl>& logger);

private:
    void start();
    void shutdown();
    void worker_loop(uint32_t idx);

private:
    common::container::ConcurrentBlockingQueue<LogTask> _buffer;
    uint32_t _threadCnt{0};
    std::vector<std::thread> _threadPool;
    std::string _paramStr;
};
}  // namespace origin::logging

#endif  // ORIGIN_LOGGING_INTERNAL_TASK_POOL_H
