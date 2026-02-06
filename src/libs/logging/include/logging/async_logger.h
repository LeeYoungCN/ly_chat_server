#ifndef LOGGING_ASYNC_LOGGER_H
#define LOGGING_ASYNC_LOGGER_H

#include <memory>
#include <utility>
#include <vector>

#include "logging/details/log_task_scheduler.h"
#include "logging/logger.h"

namespace logging {

namespace details {
class LogTaskScheduler;
}

class AsyncLogger : public Logger, public std::enable_shared_from_this<AsyncLogger> {
    friend class details::LogTaskScheduler;

public:
    AsyncLogger() = default;
    ~AsyncLogger() override = default;

    AsyncLogger(std::string name, std::shared_ptr<Sink> sink,
                std::weak_ptr<details::LogTaskScheduler> scheduler);

    AsyncLogger(std::string name, std::vector<std::shared_ptr<Sink>> sinks,
                std::weak_ptr<details::LogTaskScheduler> scheduler);

    AsyncLogger(std::string name, std::initializer_list<std::shared_ptr<Sink>> sinks,
                std::weak_ptr<details::LogTaskScheduler> scheduler);

    template <typename It>
    AsyncLogger(std::string name, It begin, It end,
                std::weak_ptr<details::LogTaskScheduler> scheduler)
        : Logger(std::move(name), begin, end), _scheduler(std::move(scheduler))
    {
    }

protected:
    void sink_it(const details::LogMsg& logMsg) override;
    void flush_it() override;

protected:  // friend
    void backend_log(const details::LogMsg& logMsg);
    void backend_flush();

private:
    std::weak_ptr<details::LogTaskScheduler> _scheduler;
};
}  // namespace logging

#endif  // LOGGING_ASYNC_LOGGER_H
