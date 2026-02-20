#pragma once
#ifndef LOGGING_DETAILS_LOG_THREAD_POOL_H
#define LOGGING_DETAILS_LOG_THREAD_POOL_H

#include <cstdint>
#include <memory>

#include "common/compiler/macros.h"
#include "logging/details/log_msg.h"
#include "logging/details/log_task.h"

namespace logging {
class AsyncLogger;

namespace details {

class COMMON_API TaskPool {
public:
    TaskPool();
    ~TaskPool();
    explicit TaskPool(uint32_t capacity);
    TaskPool(uint32_t capacity, uint32_t threadCnt);

    void log(const std::shared_ptr<AsyncLogger>& logger, const LogMsg& logMsg);
    void flush(const std::shared_ptr<AsyncLogger>& logger);

protected:
    void worker_loop(uint32_t idx);

private:
    struct Impl;
    std::unique_ptr<Impl> _pimpl;
};
}  // namespace details
}  // namespace logging

#endif  // LOGGING_DETAILS_LOG_THREAD_POOL_H
