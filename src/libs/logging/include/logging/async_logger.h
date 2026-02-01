#ifndef LOGGING_ASYNC_LOGGER_H
#define LOGGING_ASYNC_LOGGER_H

#include <memory>

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

    explicit AsyncLogger(std::weak_ptr<details::LogTaskScheduler> msgPool);
    AsyncLogger(std::string name, std::weak_ptr<details::LogTaskScheduler> msgPool);

protected:
    void sinks_log(const details::LogMsg& logMsg);
    void sinks_flush();
    void log_it(details::LogMsg&& logMsg) override;
    void flush_it() override;

private:
    std::weak_ptr<details::LogTaskScheduler> _msgPool;
};
}  // namespace logging

#endif  // LOGGING_ASYNC_LOGGER_H
