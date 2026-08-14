#ifndef LOGGING_LOGGERS_INTERNAL_ASYNC_LOGGER_IMPL_H
#define LOGGING_LOGGERS_INTERNAL_ASYNC_LOGGER_IMPL_H

#include <initializer_list>
#include <memory>
#include <string_view>
#include <vector>

#include "internal/task_pool.h"
#include "loggers/internal/logger_impl.h"
#include "logging/log_msg.h"
#include "logging/sinks/sink.h"

namespace logging {
class AsyncLoggerImpl : public LoggerImpl, public std::enable_shared_from_this<AsyncLoggerImpl> {
public:
    AsyncLoggerImpl() = delete;
    ~AsyncLoggerImpl() override = default;

    AsyncLoggerImpl(std::string_view name, const std::shared_ptr<logging::Sink>& sink);
    AsyncLoggerImpl(std::string_view name,
                    const std::vector<std::shared_ptr<logging::Sink>>& sinks);
    AsyncLoggerImpl(std::string_view name,
                    const std::initializer_list<std::shared_ptr<logging::Sink>>& sinks);

    AsyncLoggerImpl(std::string_view name, const std::shared_ptr<logging::Sink>& sink,
                    const std::weak_ptr<logging::TaskPool>& pool);
    AsyncLoggerImpl(std::string_view name, const std::vector<std::shared_ptr<logging::Sink>>& sinks,
                    const std::weak_ptr<logging::TaskPool>& pool);
    AsyncLoggerImpl(std::string_view name,
                    const std::initializer_list<std::shared_ptr<logging::Sink>>& sinks,
                    const std::weak_ptr<logging::TaskPool>& pool);

protected:
    void log_it(const LogMsg& logMsg) override;
    void flush_it() override;

private:
    std::weak_ptr<logging::TaskPool> _taskPool;
};
}  // namespace logging

#endif  // LOGGING_LOGGERS_INTERNAL_ASYNC_LOGGER_IMPL_H
