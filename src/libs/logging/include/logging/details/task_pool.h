#pragma once
#ifndef LOGGING_DETAILS_LOG_THREAD_POOL_H
#define LOGGING_DETAILS_LOG_THREAD_POOL_H

#include <cstddef>
#include <cstdint>
#include <memory>

#include "logging/details/log_msg.h"

namespace logging {
class AsyncLoggerImpl;
namespace details {

class TaskPool {
public:
    static constexpr uint32_t DEFAULT_CAPACITY = 4096;
    static constexpr uint32_t DEFAULT_THREAD_CNT = 1;
public:
    TaskPool();
    ~TaskPool();
    explicit TaskPool(uint32_t capacity);
    TaskPool(uint32_t capacity, uint32_t threadCnt);

    void log(const std::shared_ptr<AsyncLoggerImpl>& logger, const LogMsg& logMsg);
    void flush(const std::shared_ptr<AsyncLoggerImpl>& logger);
    [[nodiscard]] size_t task_count() const;

    void start();
    void shutdown();

protected:
    void worker_loop(uint32_t idx);

private:
    struct Impl;
    std::unique_ptr<Impl> _pimpl;
};
}  // namespace details
}  // namespace logging

#endif  // LOGGING_DETAILS_LOG_THREAD_POOL_H
