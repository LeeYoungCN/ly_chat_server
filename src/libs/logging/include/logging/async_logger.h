#ifndef LOGGING_ASYNC_LOGGER_H
#define LOGGING_ASYNC_LOGGER_H

#include <memory>
#include <utility>
#include <vector>

#include "logging/details/log_thread_pool.h"
#include "logging/logger.h"

namespace logging {

namespace details {
class LogThreadPool;
}

class AsyncLogger : public Logger, public std::enable_shared_from_this<AsyncLogger> {
    friend class details::LogThreadPool;

public:
    AsyncLogger() = default;
    ~AsyncLogger() override;

    AsyncLogger(std::string name, std::shared_ptr<Sink> sink,
                std::weak_ptr<details::LogThreadPool> pool);

    AsyncLogger(std::string name, std::vector<std::shared_ptr<Sink>> sinks,
                std::weak_ptr<details::LogThreadPool> pool);

    AsyncLogger(std::string name, std::initializer_list<std::shared_ptr<Sink>> sinks,
                std::weak_ptr<details::LogThreadPool> pool);

    template <typename It>
    AsyncLogger(std::string name, It begin, It end, std::weak_ptr<details::LogThreadPool> pool)
        : Logger(std::move(name), begin, end), _threadPool(std::move(pool))
    {
    }

protected:
    void sink_it(const details::LogMsg& logMsg) override;
    void flush_it() override;

protected:  // friend
    void backend_log(const details::LogMsg& logMsg);
    void backend_flush();

private:
    std::weak_ptr<details::LogThreadPool> _threadPool;
};
}  // namespace logging

#endif  // LOGGING_ASYNC_LOGGER_H
