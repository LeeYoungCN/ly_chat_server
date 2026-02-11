#include "logging/async_logger.h"

#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/details/task_pool.h"
#include "logging/logger.h"

namespace logging {

AsyncLogger::~AsyncLogger()
{
    _taskPool.reset();
    DEBUG_LOGGER_INFO("Async logger release. [{}].", name());
}

AsyncLogger::AsyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink,
                         std::weak_ptr<details::TaskPool> pool)
    : Logger(name, {sink}), _taskPool(std::move(pool))
{
}

AsyncLogger::AsyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks,
                         std::weak_ptr<details::TaskPool> pool)
    : Logger(name, sinks), _taskPool(std::move(pool))
{
}

AsyncLogger::AsyncLogger(std::string_view name,
                         const std::initializer_list<std::shared_ptr<Sink>>& sinks,
                         std::weak_ptr<details::TaskPool> pool)
    : Logger(name, sinks), _taskPool(std::move(pool))
{
}

void AsyncLogger::sink_it(const details::LogMsg& logMsg)
{
    _taskPool.lock()->log(shared_from_this(), logMsg);
}

void AsyncLogger::flush_it()
{
    _taskPool.lock()->flush(shared_from_this());
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
