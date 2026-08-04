#include "internal/loggers/async_logger_impl.h"

#include <initializer_list>
#include <memory>
#include <string_view>
#include <vector>

#include "internal/loggers/logger_impl.h"
#include "logging/details/log_msg.h"
#include "logging/details/registry.h"
#include "logging/details/task_pool.h"
#include "logging/sinks/sink.h"

namespace logging {

AsyncLoggerImpl::AsyncLoggerImpl(std::string_view name, const std::shared_ptr<logging::Sink>& sink)
    : LoggerImpl(name, sink)
{
    if (details::Registry::instance().get_task_pool() == nullptr) {
        throw std::runtime_error(
            "Async logger requires a task pool to be registered in the registry.");
    }
    _taskPool = details::Registry::instance().get_task_pool();
}

AsyncLoggerImpl::AsyncLoggerImpl(std::string_view name,
                                 const std::vector<std::shared_ptr<logging::Sink>>& sinks)
    : LoggerImpl(name, sinks)
{
    if (details::Registry::instance().get_task_pool() == nullptr) {
        throw std::runtime_error(
            "Async logger requires a task pool to be registered in the registry.");
    }
    _taskPool = details::Registry::instance().get_task_pool();
}

AsyncLoggerImpl::AsyncLoggerImpl(std::string_view name,
                                 const std::initializer_list<std::shared_ptr<logging::Sink>>& sinks)
    : LoggerImpl(name, sinks)
{
    if (details::Registry::instance().get_task_pool() == nullptr) {
        throw std::runtime_error(
            "Async logger requires a task pool to be registered in the registry.");
    }
    _taskPool = details::Registry::instance().get_task_pool();
}

AsyncLoggerImpl::AsyncLoggerImpl(std::string_view name, const std::shared_ptr<logging::Sink>& sink,
                                 const std::weak_ptr<logging::details::TaskPool>& pool)
    : LoggerImpl(name, sink), _taskPool(pool)
{
    if (pool.expired()) {
        throw std::invalid_argument("Task pool cannot be null.");
    }
}

AsyncLoggerImpl::AsyncLoggerImpl(std::string_view name,
                                 const std::vector<std::shared_ptr<logging::Sink>>& sinks,
                                 const std::weak_ptr<logging::details::TaskPool>& pool)
    : LoggerImpl(name, sinks), _taskPool(pool)
{
    if (pool.expired()) {
        throw std::invalid_argument("Task pool cannot be null.");
    }
}

AsyncLoggerImpl::AsyncLoggerImpl(std::string_view name,
                                 const std::initializer_list<std::shared_ptr<logging::Sink>>& sinks,
                                 const std::weak_ptr<logging::details::TaskPool>& pool)
    : LoggerImpl(name, sinks), _taskPool(pool)
{
    if (pool.expired()) {
        throw std::invalid_argument("Task pool cannot be null.");
    }
}

void AsyncLoggerImpl::log_it(const details::LogMsg& logMsg)
{
    _taskPool.lock()->log(shared_from_this(), logMsg);
}

void AsyncLoggerImpl::flush_it()
{
    _taskPool.lock()->flush(shared_from_this());
}
}  // namespace logging
