#include "logging/loggers/async_logger.h"

#include <memory>
#include <vector>

#include "internal/loggers/async_logger_impl.h"
#include "logging/details/task_pool.h"
#include "logging/loggers/logger_base.h"

namespace logging {

using namespace logging::details;

AsyncLogger::AsyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink)
    : LoggerBase(std::make_shared<AsyncLoggerImpl>(name, sink))
{
}

AsyncLogger::AsyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks)
    : LoggerBase(std::make_shared<AsyncLoggerImpl>(name, sinks))
{
}

AsyncLogger::AsyncLogger(std::string_view name,
                         const std::initializer_list<std::shared_ptr<Sink>>& sinks)
    : LoggerBase(std::make_shared<AsyncLoggerImpl>(name, sinks))
{
}

AsyncLogger::AsyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink,
                         const std::weak_ptr<logging::details::TaskPool>& pool)
    : LoggerBase(std::make_shared<AsyncLoggerImpl>(name, sink, pool))
{
}

AsyncLogger::AsyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks,
                         const std::weak_ptr<logging::details::TaskPool>& pool)
    : LoggerBase(std::make_shared<AsyncLoggerImpl>(name, sinks, pool))
{
}

AsyncLogger::AsyncLogger(std::string_view name,
                         const std::initializer_list<std::shared_ptr<Sink>>& sinks,
                         const std::weak_ptr<logging::details::TaskPool>& pool)
    : LoggerBase(std::make_shared<AsyncLoggerImpl>(name, sinks, pool))
{
}

}  // namespace logging
