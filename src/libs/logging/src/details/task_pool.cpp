#include "logging/details/task_pool.h"

#include <cstdint>
#include <thread>
#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/async_logger.h"
#include "logging/details/common.h"
#include "logging/details/log_msg.h"
#include "logging/details/log_task.h"

namespace logging::details {
using namespace common::container;

TaskPool::TaskPool()
    : TaskPool(THREAD_POOL_DEFAULT_CAPACITY, THREAD_POOL_DEFAULT_THREAD_CNT)
{
}

TaskPool::TaskPool(uint32_t capacity)
    : TaskPool(capacity, THREAD_POOL_DEFAULT_THREAD_CNT)
{
}

TaskPool::TaskPool(uint32_t capacity, uint32_t threadCnt)
    : _logBuffer(ConcurrentBlockingQueue<LogTask>(capacity)), _threadCnt(threadCnt)
{
    _threadPool.reserve(_threadCnt);
    for (uint32_t i = 0; i < _threadCnt; i++) {
        _threadPool.emplace_back(&TaskPool::worker_loop, this, i + 1);
    }
}

TaskPool::~TaskPool()
{
    for (uint32_t i = 0; i < _threadPool.size(); i++) {
        _logBuffer.enqueue(LogTask(TaskType::SHUTDOWN));
    }

    for (auto& t : _threadPool) {
        t.join();
    }
    DEBUG_LOGGER_INFO("Log thread pool release.");
}

void TaskPool::log(std::shared_ptr<AsyncLogger> logger, const LogMsg& logMsg)
{
    _logBuffer.enqueue_wait(LogTask(TaskType::LOG, std::move(logger), logMsg));
}

void TaskPool::flush(std::shared_ptr<AsyncLogger> logger)
{
    _logBuffer.enqueue_wait(LogTask(TaskType::FLUSH, std::move(logger), LogMsg()));
}

void TaskPool::worker_loop(uint32_t idx)
{
    bool isRunning = true;
    while (isRunning) {
        LogTask task;
        _logBuffer.dequeue_wait(task);

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
    }
    DEBUG_LOGGER_INFO("Log thread pool worker loop shutdown. [{}/{}]", idx, _threadCnt);
}
}  // namespace logging::details
