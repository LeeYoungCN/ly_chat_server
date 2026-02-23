#include "logging/async_logger.h"

#include <memory>
#include <utility>
#include <vector>

#include "common/debug/debug_logger.h"
#include "logging/details/task_pool.h"
#include "logging/logger.h"

namespace logging {

using namespace logging::details;

struct AsyncLogger::Impl {
    std::weak_ptr<logging::TaskPool> taskPool;

    explicit Impl(std::weak_ptr<TaskPool> pool) : taskPool(std::move(pool)) {}
};

AsyncLogger::~AsyncLogger()
{
    _pimpl->taskPool.reset();
    delete _pimpl;
    DEBUG_LOGGER_INFO("Async logger release. [{}].", name());
}

AsyncLogger::AsyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink,
                         const std::weak_ptr<TaskPool>& pool)
    : Logger(name, sink), _pimpl(new Impl(pool))
{
}

AsyncLogger::AsyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks,
                         const std::weak_ptr<TaskPool>& pool)
    : Logger(name, sinks), _pimpl(new Impl(pool))
{
}

AsyncLogger::AsyncLogger(std::string_view name,
                         const std::initializer_list<std::shared_ptr<Sink>>& sinks,
                         const std::weak_ptr<TaskPool>& pool)
    : Logger(name, sinks), _pimpl(new Impl(pool))
{
}

void AsyncLogger::sink_it(const LogMsg& logMsg)
{
    _pimpl->taskPool.lock()->log(shared_from_this(), logMsg);
}

void AsyncLogger::flush_it()
{
    _pimpl->taskPool.lock()->flush(shared_from_this());
}

void AsyncLogger::backend_log(const LogMsg& logMsg)
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
