#include "logging/details/log_task_scheduler.h"

#include <cstdint>
#include <mutex>
#include <thread>
#include <utility>

#include "logging/async_logger.h"
#include "logging/details/log_msg.h"

namespace logging::details {
LogTaskScheduler::LogTaskScheduler() : LogTaskScheduler(1, 1024) {}

LogTaskScheduler::LogTaskScheduler(uint32_t threadCnt, uint32_t bufferCapacity)
    : _threadCnt(threadCnt), _bufferCapacity(bufferCapacity)
{
    _threadPool.reserve(threadCnt);
    for (uint32_t i = 0; i < _threadCnt; i++) {
        _threadPool.emplace_back(&LogTaskScheduler::worker_loop, this);
    }
}

LogTaskScheduler::~LogTaskScheduler()
{
    {
        std::unique_lock lock(_bufferMtx);
        for (uint32_t i = 0; i < _threadCnt; i++) {
            _logBuffer.emplace(TaskType::SHUTDOWN);
        }
    }
    _bufferCv.notify_all();
    for (auto& t : _threadPool) {
        t.join();
    }
}

void LogTaskScheduler::log(logger_ptr&& logger, LogMsg&& logMsg)
{
    {
        std::unique_lock lock(_bufferMtx);
        _logBuffer.emplace(TaskType::LOG, logger, std::move(logMsg));
    }
    _bufferCv.notify_one();
}

void LogTaskScheduler::flush(logger_ptr&& logger)
{
    {
        std::unique_lock lock(_bufferMtx);
        _logBuffer.emplace(TaskType::FLUSH, logger, LogMsg());
    }
    _bufferCv.notify_one();
}

void LogTaskScheduler::worker_loop()
{
    bool isRunning = true;
    while (isRunning) {
        std::unique_lock lock(_bufferMtx);
        _bufferCv.wait(lock, [this]() { return !_logBuffer.empty(); });

        auto asyncMsg = _logBuffer.front();
        _logBuffer.pop();
        lock.unlock();

        switch (asyncMsg.type) {
            case TaskType::LOG:
                asyncMsg.logger->sinks_log(asyncMsg.logMsg);

                break;
            case TaskType::FLUSH:
                asyncMsg.logger->sinks_flush();
                break;
            case TaskType::SHUTDOWN:
            default:
                isRunning = false;
                break;
        }
    }
}
}  // namespace logging::details
