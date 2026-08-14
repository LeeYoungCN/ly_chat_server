#include "logging/loggers/sync_logger.h"

#include <memory>
#include <vector>

#include "loggers/internal/sync_logger_impl.h"
#include "logging/loggers/logger_base.h"

namespace logging {
SyncLogger::SyncLogger(std::string_view name) : LoggerBase(std::make_shared<SyncLoggerImpl>(name))
{
}

SyncLogger::SyncLogger(std::string_view name, const std::shared_ptr<Sink>& sink)
    : LoggerBase(std::make_shared<SyncLoggerImpl>(name, sink))
{
}

SyncLogger::SyncLogger(std::string_view name, const std::vector<std::shared_ptr<Sink>>& sinks)
    : LoggerBase(std::make_shared<SyncLoggerImpl>(name, sinks))
{
}

SyncLogger::SyncLogger(std::string_view name,
                       const std::initializer_list<std::shared_ptr<Sink>>& sinks)
    : LoggerBase(std::make_shared<SyncLoggerImpl>(name, std::vector<std::shared_ptr<Sink>>(sinks)))
{
}

}  // namespace logging
