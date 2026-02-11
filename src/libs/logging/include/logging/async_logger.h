#ifndef LOGGING_ASYNC_LOGGER_H
#define LOGGING_ASYNC_LOGGER_H

#include <memory>
#include <utility>
#include <vector>

#include "logging/details/task_pool.h"
#include "logging/logger.h"

namespace logging {

namespace details {
class TaskPool;
}

class AsyncLogger : public Logger, public std::enable_shared_from_this<AsyncLogger> {
    friend class logging::details::TaskPool;

public:
    AsyncLogger() = default;
    ~AsyncLogger() override;

    AsyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink,
                std::weak_ptr<logging::details::TaskPool> pool);

    AsyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks,
                std::weak_ptr<logging::details::TaskPool> pool);

    AsyncLogger(std::string_view name, const std::initializer_list<std::shared_ptr<Sink>>& sinks,
                std::weak_ptr<logging::details::TaskPool> pool);

    template <typename It>
    AsyncLogger(std::string_view name, It begin, It end, std::weak_ptr<logging::details::TaskPool> pool)
        : Logger(std::move(name), begin, end), _taskPool(std::move(pool))
    {
    }

protected:
    void sink_it(const logging::details::LogMsg& logMsg) override;
    void flush_it() override;

protected:  // friend
    void backend_log(const logging::details::LogMsg& logMsg);
    void backend_flush();

private:
    std::weak_ptr<logging::details::TaskPool> _taskPool;
};
}  // namespace logging

#endif  // LOGGING_ASYNC_LOGGER_H
