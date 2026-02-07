#include "logging/details/registry.h"

#include <memory>
#include <mutex>
#include <utility>

#include "common/debug/debug_logger.h"
#include "logging/details/common.h"
#include "logging/sinks/stdout_sink.h"

namespace logging::details {
Registry::Registry()
{
    _defaultLogger = std::make_shared<Logger>(DEFAULT_LOGGER_NAME, std::make_shared<StdoutSink>());
    _loggers[DEFAULT_LOGGER_NAME] = _defaultLogger;
}

// public
std::shared_ptr<Logger> Registry::default_logger()
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return _defaultLogger;
}

Logger* Registry::default_logger_raw()
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return _defaultLogger.get();
}

void Registry::set_default_logger(std::shared_ptr<Logger> newLogger)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    if (newLogger != nullptr) {
        _loggers[newLogger->name()] = newLogger;
    }
    _defaultLogger = std::move(newLogger);
}

bool Registry::exist(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return exist_it(name);
}

bool Registry::register_logger(std::shared_ptr<Logger> logger)
{
    return register_logger(logger->name(), std::move(logger));
}

bool Registry::register_logger(const std::string& name, std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return register_logger_it(name, std::move(logger));
}

void Registry::register_or_replace_logger(std::shared_ptr<Logger> logger)
{
    register_or_replace_logger(logger->name(), std::move(logger));
}

void Registry::register_or_replace_logger(const std::string& name, std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    _loggers[name] = std::move(logger);
}

void Registry::remove_logger(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    bool isDefaultLogger = (_defaultLogger != nullptr && _defaultLogger->name() == name);
    _loggers.erase(name);
    if (isDefaultLogger) {
        _defaultLogger.reset();
    }
}

void Registry::remove_all_logger()
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    _loggers.clear();
    _defaultLogger.reset();
}

std::shared_ptr<Logger> Registry::get(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    auto it = _loggers.find(name);
    return it == _loggers.end() ? nullptr : it->second;
}

// private
bool Registry::register_logger_it(const std::string& name, std::shared_ptr<Logger> logger)
{
    if (exist_it(name)) {
        DEBUG_LOGGER_ERR("Logger already exist. {}", name);
        return false;
    }
    _loggers[name] = std::move(logger);
    return true;
}

bool Registry::exist_it(const std::string& name)
{
    return (_loggers.find(name) != _loggers.end());
}

}  // namespace logging::details
