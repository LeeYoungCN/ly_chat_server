#include "logging/async_logger.h"

#include <utility>

#include "logging/details/log_task_scheduler.h"
#include "logging/logger.h"

namespace logging {

AsyncLogger::AsyncLogger(std::string name, std::shared_ptr<Sink> sink,
                         std::weak_ptr<details::LogTaskScheduler> scheduler)
    : Logger(std::move(name), {std::move(sink)}), _scheduler(std::move(scheduler))
{
}

AsyncLogger::AsyncLogger(std::string name, std::vector<std::shared_ptr<Sink>> sinks,
                         std::weak_ptr<details::LogTaskScheduler> scheduler)
    : Logger(std::move(name), std::move(sinks)), _scheduler(std::move(scheduler))
{
}

AsyncLogger::AsyncLogger(std::string name, std::initializer_list<std::shared_ptr<Sink>> sinks,
                         std::weak_ptr<details::LogTaskScheduler> scheduler)
    : Logger(std::move(name), sinks), _scheduler(std::move(scheduler))
{
}

void AsyncLogger::sink_it(const details::LogMsg& logMsg)
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

    if (should_flush(logMsg.level)) {
        sinks_flush_it();
    }
}

void AsyncLogger::backend_flush()
{
    sinks_flush_it();
}

}  // namespace logging
