#include "logging/async_logger.h"

#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/details/log_thread_pool.h"
#include "logging/logger.h"

namespace logging {

AsyncLogger::~AsyncLogger()
{
    _threadPool.reset();
    DEBUG_LOGGER_INFO("Async logger release. [{}].", name());
}

AsyncLogger::AsyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink,
                         std::weak_ptr<details::LogThreadPool> pool)
    : Logger(name, {sink}), _threadPool(std::move(pool))
{
}

AsyncLogger::AsyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks,
                         std::weak_ptr<details::LogThreadPool> pool)
    : Logger(name, sinks), _threadPool(std::move(pool))
{
}

AsyncLogger::AsyncLogger(std::string_view name,
                         const std::initializer_list<std::shared_ptr<Sink>>& sinks,
                         std::weak_ptr<details::LogThreadPool> pool)
    : Logger(name, sinks), _threadPool(std::move(pool))
{
}

void AsyncLogger::sink_it(const details::LogMsg& logMsg)
{
    _threadPool.lock()->log(shared_from_this(), logMsg);
}

void AsyncLogger::flush_it()
{
    _threadPool.lock()->flush(shared_from_this());
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
