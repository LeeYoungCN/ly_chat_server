#ifndef LOGGING_LOGGERS_SYNC_LOGGER_H
#define LOGGING_LOGGERS_SYNC_LOGGER_H

#include <initializer_list>
#include <memory>
#include <string_view>
#include <vector>

#include "logging/loggers/logger_base.h"
#include "logging/sinks/sink.h"

namespace logging {
class SyncLogger : public LoggerBase {
public:
    SyncLogger() = delete;
    ~SyncLogger() override = default;

    explicit SyncLogger(std::string_view name);

    SyncLogger(std::string_view name, const std::shared_ptr<logging::Sink>& sink);

    SyncLogger(std::string_view name, const std::vector<std::shared_ptr<logging::Sink>>& sinks);

    SyncLogger(std::string_view name,
               const std::initializer_list<std::shared_ptr<logging::Sink>>& sinks);
};
}  // namespace logging

#endif  // LOGGING_LOGGERS_SYNC_LOGGER_H
