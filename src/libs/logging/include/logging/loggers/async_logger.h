#ifndef LOGGING_LOGGERS_ASYNC_LOGGER_H
#define LOGGING_LOGGERS_ASYNC_LOGGER_H

#include <memory>

#include "logging/details/task_pool.h"
#include "logging/loggers/logger_base.h"

namespace logging {

class AsyncLogger : public LoggerBase {
public:
    AsyncLogger() = delete;
    ~AsyncLogger() override = default;

    AsyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink);

    AsyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks);

    AsyncLogger(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks);

    AsyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink,
                const std::weak_ptr<logging::details::TaskPool>& pool);

    AsyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks,
                const std::weak_ptr<logging::details::TaskPool>& pool);

    AsyncLogger(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks,
                const std::weak_ptr<logging::details::TaskPool>& pool);
};
}  // namespace logging

#endif  // LOGGING_LOGGERS_ASYNC_LOGGER_H
