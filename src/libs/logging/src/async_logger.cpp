#include "logging/async_logger.h"

#include <utility>

#include "logging/details/log_task_scheduler.h"
#include "logging/logger.h"

namespace logging {

AsyncLogger::AsyncLogger(std::string name, std::weak_ptr<details::LogTaskScheduler> scheduler)
    : Logger(std::move(name)), _scheduler(std::move(scheduler))
{
}

void AsyncLogger::log_it(const details::LogMsg& logMsg)
{
    _scheduler.lock()->log(shared_from_this(), logMsg);
}

void AsyncLogger::flush_it()
{
    _scheduler.lock()->flush(shared_from_this());
}

void AsyncLogger::backend_log(const details::LogMsg& logMsg)
{
    sinks_log_it(logMsg);
}

void AsyncLogger::backend_flush()
{
    sinks_flush_it();
}

}  // namespace logging
