#include "logging/details/log_task_scheduler.h"

#include <cstdint>
#include <thread>
#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/async_logger.h"
#include "logging/details/log_msg.h"

namespace logging::details {
LogTaskScheduler::LogTaskScheduler() : LogTaskScheduler(1, 1024) {}

LogTaskScheduler::LogTaskScheduler(uint32_t threadCnt, uint32_t bufferCapacity)
    : _logBuffer(common::container::ConcurrentBlockingQueue<LogTask>(bufferCapacity))
{
    _threadPool.reserve(threadCnt);
    for (uint32_t i = 0; i < threadCnt; i++) {
        _threadPool.emplace_back(&LogTaskScheduler::worker_loop, this);
    }
}

LogTaskScheduler::~LogTaskScheduler()
{
    for (uint32_t i = 0; i < _threadPool.size(); i++) {
        _logBuffer.enqueue(LogTask(TaskType::SHUTDOWN));
    }

    for (auto& t : _threadPool) {
        t.join();
    }
}

void LogTaskScheduler::log(LoggerPtr&& logger, LogMsg&& logMsg)
{
    _logBuffer.enqueue(LogTask(TaskType::LOG, logger, std::move(logMsg)));
}

void LogTaskScheduler::flush(LoggerPtr&& logger)
{
    _logBuffer.enqueue(LogTask(TaskType::FLUSH, logger, LogMsg()));
}

void LogTaskScheduler::worker_loop()
{
    bool isRunning = true;
    while (isRunning) {
        LogTask task;
        _logBuffer.dequeue_wait(task);

        switch (task.type) {
            case TaskType::LOG:
                task.logger->sinks_log(task.logMsg);
                break;
            case TaskType::FLUSH:
                task.logger->sinks_flush();
                break;
            case TaskType::SHUTDOWN:
            default:
                isRunning = false;
                break;
        }
    }
    DEBUG_LOGGER_INFO("Worker shutdown.")
}
}  // namespace logging::details
