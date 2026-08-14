#include "internal/task_pool.h"

#include <atomic>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "common/container/concurrent_blocking_queue.hpp"
#include "common/debug/debug_logger.h"
#include "internal/log_task.h"
#include "internal/loggers/async_logger_impl.h"
#include "logging/log_msg.h"

namespace logging {
using namespace common::container;

TaskPool::TaskPool() : TaskPool(TaskPool::DEFAULT_CAPACITY, TaskPool::DEFAULT_THREAD_CNT) {}

TaskPool::TaskPool(uint32_t capacity) : TaskPool(capacity, TaskPool::DEFAULT_THREAD_CNT) {}

TaskPool::TaskPool(uint32_t capacity, uint32_t threadCnt)
    : _buffer(ConcurrentBlockingQueue<LogTask>(capacity)), _threadCnt(threadCnt)
{
    if (capacity == 0) {
        throw std::invalid_argument("Capacity must be greater than zero.");
    }

    if (_threadCnt == 0) {
        throw std::invalid_argument("Thread count must be greater than zero.");
    }
    _threadPool.reserve(_threadCnt);
    start();
}

TaskPool::~TaskPool()
{
    shutdown();
    DEBUG_LOGGER_DBG("Log thread pool release.");
}

void TaskPool::log(const std::shared_ptr<AsyncLoggerImpl>& logger, const LogMsg& logMsg)
{
    if (!_isThreadRunning.load()) {
        DEBUG_LOGGER_ERR("Log failed. Task pool shutdown");
    }
    _buffer.enqueue_wait(LogTask(TaskType::LOG, logger, logMsg));
}

void TaskPool::flush(const std::shared_ptr<AsyncLoggerImpl>& logger)
{
    if (!_isThreadRunning.load()) {
        DEBUG_LOGGER_ERR("Flush failed. Task pool shutdown");
    }
    _buffer.enqueue_wait(LogTask(TaskType::FLUSH, logger, LogMsg()));
}

[[nodiscard]] size_t TaskPool::task_count()
{
    return _buffer.size();
}

void TaskPool::start()
{
    if (_isThreadRunning.load()) {
        DEBUG_LOGGER_DBG("Task pool already running.");
        return;
    }
    {
        std::lock_guard<std::mutex> lock(_threadPoolMtx);
        if (_isThreadRunning.load()) {
            return;
        }
        _isThreadRunning.store(true);

        for (uint32_t i = 0; i < _threadCnt; i++) {
            _threadPool.emplace_back(&TaskPool::worker_loop, this, i + 1);
        }
    }

    DEBUG_LOGGER_TRACE("Task pool start success.");
}

void TaskPool::shutdown()
{
    if (!_isThreadRunning.load()) {
        DEBUG_LOGGER_DBG("Task pool already shutdown.");
        return;
    }
    {
        std::lock_guard<std::mutex> lock(_threadPoolMtx);

        if (!_isThreadRunning.load()) {
            return;
        }
        _isThreadRunning.store(false);

        for (uint32_t i = 0; i < _threadPool.size(); i++) {
            _buffer.enqueue_wait(LogTask(TaskType::SHUTDOWN));
        }

        for (auto& t : _threadPool) {
            if (t.joinable()) {
                t.join();
            }
        }

        if (!_buffer.empty()) {
            DEBUG_LOGGER_ERR("Buffer not empty. size: {}.", _buffer.size())
        }

        _threadPool.clear();
    }

    DEBUG_LOGGER_DBG("Task pool shutdown success.");
}

void TaskPool::worker_loop(uint32_t idx)
{
    DEBUG_LOGGER_DBG("Log thread pool worker loop start. [{}/{}]", idx, _threadCnt);
    bool isRunning = true;
    while (isRunning) {
        LogTask task;
        _buffer.dequeue_wait(task);
        try {
            switch (task.type) {
                case TaskType::LOG:
                    task.logger->backend_log(task.logMsg);
                    break;
                case TaskType::FLUSH:
                    task.logger->backend_flush();
                    break;
                case TaskType::SHUTDOWN:
                default:
                    isRunning = false;
                    break;
            }
        } catch (std::exception& ex) {
            DEBUG_LOGGER_ERR("[Exception]: {}.", ex.what());
        }
    }
    DEBUG_LOGGER_DBG("Log task pool worker loop shutdown. [{}/{}]", idx, _threadCnt);
}
}  // namespace logging
