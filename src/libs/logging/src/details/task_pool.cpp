#include "logging/details/task_pool.h"

#include <cstdint>
#include <exception>
#include <memory>
#include <thread>
#include <vector>

#include "common/container/concurrent_blocking_queue.hpp"
#include "common/debug/debug_logger.h"
#include "logging/async_logger.h"
#include "logging/details/common.h"
#include "logging/details/log_msg.h"
#include "logging/details/log_task.h"

namespace logging::details {
using namespace common::container;

struct TaskPool::Impl {
    ConcurrentBlockingQueue<LogTask> buffer;
    uint32_t threadCnt = 0;
    std::vector<std::thread> threadPool;

    Impl(uint32_t capacity, uint32_t threadCnt)
        : buffer(capacity), threadCnt(threadCnt), threadPool(threadCnt)
    {
    }
};

TaskPool::TaskPool() : TaskPool(THREAD_POOL_DEFAULT_CAPACITY, THREAD_POOL_DEFAULT_THREAD_CNT) {}

TaskPool::TaskPool(uint32_t capacity) : TaskPool(capacity, THREAD_POOL_DEFAULT_THREAD_CNT) {}

TaskPool::TaskPool(uint32_t capacity, uint32_t threadCnt) : _pimpl(new Impl(capacity, threadCnt))
{
    _pimpl->threadPool.reserve(_pimpl->threadCnt);
    for (uint32_t i = 0; i < _pimpl->threadCnt; i++) {
        _pimpl->threadPool.emplace_back(&TaskPool::worker_loop, this, i + 1);
    }
}

TaskPool::~TaskPool()
{
    for (uint32_t i = 0; i < _pimpl->threadPool.size(); i++) {
        _pimpl->buffer.enqueue(LogTask(TaskType::SHUTDOWN));
    }

    for (auto& t : _pimpl->threadPool) {
        if (t.joinable()) {
            t.join();
        }
    }
    delete _pimpl;
    DEBUG_LOGGER_INFO("Log thread pool release.");
}

void TaskPool::log(const std::shared_ptr<AsyncLogger>& logger, const LogMsg& logMsg)
{
    _pimpl->buffer.enqueue_wait(LogTask(TaskType::LOG, logger, logMsg));
}

void TaskPool::flush(const std::shared_ptr<AsyncLogger>& logger)
{
    _pimpl->buffer.enqueue_wait(LogTask(TaskType::FLUSH, logger, LogMsg()));
}

void TaskPool::worker_loop(uint32_t idx)
{
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
    DEBUG_LOGGER_INFO("Log thread pool worker loop shutdown. [{}/{}]", idx, _pimpl->threadCnt);
}
}  // namespace logging::details
