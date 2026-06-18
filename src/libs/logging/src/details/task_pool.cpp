#include "logging/details/task_pool.h"

#include <atomic>
#include <cstdint>
#include <exception>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "common/container/concurrent_blocking_queue.hpp"
#include "common/debug/debug_logger.h"
#include "logging/async_logger.h"
#include "logging/details/constants.h"
#include "logging/details/log_msg.h"
#include "logging/details/log_task.h"

namespace logging::details {
using namespace common::container;

struct TaskPool::Impl {
    ConcurrentBlockingQueue<LogTask> buffer;
    uint32_t threadCnt = 0;
    std::vector<std::thread> threadPool;
    std::mutex threadPoolMtx;
    std::atomic<bool> isThreadRunning{false};

    Impl(uint32_t capacity, uint32_t threadCnt) : buffer(capacity), threadCnt(threadCnt) {}
};

TaskPool::TaskPool() : TaskPool(THREAD_POOL_DEFAULT_CAPACITY, THREAD_POOL_DEFAULT_THREAD_CNT) {}

TaskPool::TaskPool(uint32_t capacity) : TaskPool(capacity, THREAD_POOL_DEFAULT_THREAD_CNT) {}

TaskPool::TaskPool(uint32_t capacity, uint32_t threadCnt)
    : _pimpl(std::make_unique<Impl>(capacity, threadCnt))
{
    _pimpl->threadPool.reserve(_pimpl->threadCnt);
    start();
}

TaskPool::~TaskPool()
{
    shutdown();
    _pimpl.reset();
    DEBUG_LOGGER_DBG("Log thread pool release.");
}

void TaskPool::log(const std::shared_ptr<AsyncLogger>& logger, const LogMsg& logMsg)
{
    if (!_pimpl->isThreadRunning.load()) {
        DEBUG_LOGGER_ERR("Log failed. Task pool shutdown");
    }
    _pimpl->buffer.enqueue_wait(LogTask(TaskType::LOG, logger, logMsg));
}

void TaskPool::flush(const std::shared_ptr<AsyncLogger>& logger)
{
    if (!_pimpl->isThreadRunning.load()) {
        DEBUG_LOGGER_ERR("Flush failed. Task pool shutdown");
    }
    _pimpl->buffer.enqueue_wait(LogTask(TaskType::FLUSH, logger, LogMsg()));
}

[[nodiscard]] size_t TaskPool::task_count() const
{
    return _pimpl->buffer.size();
}

void TaskPool::start()
{
    if (_pimpl->isThreadRunning.load()) {
        DEBUG_LOGGER_DBG("Task pool already running.");
        return;
    }
    {
        std::lock_guard<std::mutex> lock(_pimpl->threadPoolMtx);
        if (_pimpl->isThreadRunning.load()) {
            return;
        }
        _pimpl->isThreadRunning.store(true);

        for (uint32_t i = 0; i < _pimpl->threadCnt; i++) {
            _pimpl->threadPool.emplace_back(&TaskPool::worker_loop, this, i + 1);
        }
    }

    DEBUG_LOGGER_TRACE("Task pool start success.");
}

void TaskPool::shutdown()
{
    if (!_pimpl->isThreadRunning.load()) {
        DEBUG_LOGGER_DBG("Task pool already shutdown.");
        return;
    }
    {
        std::lock_guard<std::mutex> lock(_pimpl->threadPoolMtx);

        if (!_pimpl->isThreadRunning.load()) {
            return;
        }
        _pimpl->isThreadRunning.store(false);

        for (uint32_t i = 0; i < _pimpl->threadPool.size(); i++) {
            _pimpl->buffer.enqueue_wait(LogTask(TaskType::SHUTDOWN));
        }

        for (auto& t : _pimpl->threadPool) {
            if (t.joinable()) {
                t.join();
            }
        }

        if (!_pimpl->buffer.empty()) {
            DEBUG_LOGGER_ERR("Buffer not empty. size: {}.", _pimpl->buffer.size())
        }

        _pimpl->threadPool.clear();
    }

    DEBUG_LOGGER_DBG("Task pool shutdown success.");
}

void TaskPool::worker_loop(uint32_t idx)
{
    DEBUG_LOGGER_DBG("Log thread pool worker loop start. [{}/{}]", idx, _pimpl->threadCnt);
    bool isRunning = true;
    while (isRunning) {
        LogTask task;
        _pimpl->buffer.dequeue_wait(task);
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
    DEBUG_LOGGER_DBG("Log task pool worker loop shutdown. [{}/{}]", idx, _pimpl->threadCnt);
}
}  // namespace logging::details
