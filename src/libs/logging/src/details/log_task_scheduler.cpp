#include "logging/details/log_task_scheduler.h"

#include <cstdint>
#include <thread>
#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/async_logger.h"
#include "logging/details/log_msg.h"
#include "logging/details/log_task.h"

namespace {
constexpr uint32_t DEFAULT_CAPACITY = 4096;
}

namespace logging::details {
using namespace common::container;
LogTaskScheduler::LogTaskScheduler() : _logBuffer(ConcurrentBlockingQueue<LogTask>(DEFAULT_CAPACITY))
{
    _threadPool.emplace_back(&LogTaskScheduler::worker_loop, this);
}

LogTaskScheduler::LogTaskScheduler(uint32_t threadCnt, uint32_t bufferCapacity)
    : _logBuffer(ConcurrentBlockingQueue<LogTask>(bufferCapacity))
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
    DEBUG_LOGGER_INFO("Log task scheduler release.");
}

void LogTaskScheduler::log(std::shared_ptr<AsyncLogger> logger, const LogMsg& logMsg)
{
    _logBuffer.enqueue_wait(LogTask(TaskType::LOG, std::move(logger), logMsg));
}

void LogTaskScheduler::flush(std::shared_ptr<AsyncLogger> logger)
{
    _logBuffer.enqueue_wait(LogTask(TaskType::FLUSH, std::move(logger), LogMsg()));
}

void LogTaskScheduler::worker_loop()
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
    DEBUG_LOGGER_INFO("Log task scheduler worker loop shutdown.");
}
}  // namespace logging::details
