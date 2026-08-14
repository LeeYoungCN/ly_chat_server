#ifndef LOGGING_LOGGERS_INTERNAL_SYNC_LOGGER_IMPL_H
#define LOGGING_LOGGERS_INTERNAL_SYNC_LOGGER_IMPL_H

#include <initializer_list>
#include <memory>
#include <string_view>
#include <vector>

#include "loggers/internal/logger_impl.h"
#include "logging/log_msg.h"
#include "logging/sinks/sink.h"

namespace logging {
class SyncLoggerImpl : public LoggerImpl {
public:
    SyncLoggerImpl() = delete;
    ~SyncLoggerImpl() override = default;

    explicit SyncLoggerImpl(std::string_view name);
    SyncLoggerImpl(std::string_view name, const std::shared_ptr<logging::Sink>& sink);
    SyncLoggerImpl(std::string_view name, const std::vector<std::shared_ptr<logging::Sink>>& sinks);
    SyncLoggerImpl(std::string_view name,
                   const std::initializer_list<std::shared_ptr<logging::Sink>>& sinks);

protected:
    void log_it(const LogMsg& logMsg) override;
    void flush_it() override;
};
}  // namespace logging

#endif  // LOGGING_LOGGERS_INTERNAL_SYNC_LOGGER_IMPL_H
