#include "logging/async_logger.h"

#include <utility>

#include "logging/details/log_task_scheduler.h"
#include "logging/logger.h"

namespace logging {

AsyncLogger::AsyncLogger(std::weak_ptr<details::LogTaskScheduler> msgPool) : _msgPool(std::move(msgPool)) {}

AsyncLogger::AsyncLogger(std::string name, std::weak_ptr<details::LogTaskScheduler> msgPool)
    : Logger(std::move(name)), _msgPool(std::move(msgPool))
{
}

void AsyncLogger::log_it(details::LogMsg&& logMsg)
{
    _msgPool.lock()->log(shared_from_this(), std::move(logMsg));
}

void AsyncLogger::flush_it()
{
    _msgPool.lock()->flush(shared_from_this());
}

void AsyncLogger::sinks_log(const details::LogMsg& logMsg)
{
    for (const auto& sink : _sinkList) {
        if (sink->should_log(logMsg.level)) {
            sink->log(logMsg);
        }
    }
}
void AsyncLogger::sinks_flush()
{
    for (const auto& sink : _sinkList) {
        sink->flush();
    }
}

}  // namespace logging
