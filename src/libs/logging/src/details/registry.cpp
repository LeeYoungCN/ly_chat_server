#include "logging/details/registry.h"

#include <memory>
#include <mutex>
#include <utility>

#include "common/debug/debug_log.h"
#include "common/debug/debug_logger.h"
#include "logging/details/common.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/sinks/stdout_sink.h"

namespace logging::details {
Registry::Registry() : _globalFormatter(new PatternFormatter())
{
    _root = std::make_shared<Logger>(ROOT_LOGGER_NAME, std::make_shared<StdoutSink>());
    _loggers[ROOT_LOGGER_NAME] = _root;
}

#pragma region Root logger
std::shared_ptr<Logger> Registry::root_logger()
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return _root;
}

Logger* Registry::root_logger_raw()
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return _root.get();
}

void Registry::set_root_logger(std::shared_ptr<Logger> newLogger)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    if (newLogger != nullptr) {
        register_or_replace_logger_it(newLogger);
    }
    _root = std::move(newLogger);
}
#pragma endregion

#pragma region Module manager
void Registry::initialize_logger(const std::shared_ptr<Logger>& logger, bool autoRegister)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    logger->set_formatter(_globalFormatter->clone());
    logger->set_level(_globalLevel);
    logger->flush_on(_globalFlushLevel);

    if (autoRegister) {
        register_or_replace_logger_it(logger);
    }
}

void Registry::set_level_all(LogLevel level)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    _globalLevel = level;

    for (auto& [_, logger] : _loggers) {
        logger->set_level(_globalLevel);
    }
}

void Registry::flush_on_all(LogLevel level)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    _globalFlushLevel = level;

    for (auto& [_, logger] : _loggers) {
        logger->flush_on(_globalFlushLevel);
    }
}

void Registry::set_pattern_all(std::string_view pattern, std::string_view timePattern)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    set_formatter_all(std::make_unique<PatternFormatter>(pattern, timePattern));
}

void Registry::set_formatter_all(std::unique_ptr<Formatter> formatter)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    _globalFormatter = std::move(formatter);
    for (auto& [_, logger] : _loggers) {
        logger->set_formatter(_globalFormatter->clone());
    }
}

void Registry::flush_all()
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    for (auto& [_, logger] : _loggers) {
        logger->flush();
    }
}

void Registry::shut_down()
{
    remove_all();
    {
        std::lock_guard<std::recursive_mutex> lock(_taskPoolMtx);
        if (_globalTaskPool != nullptr) {
            _globalTaskPool.reset();
        }
    }
    DEBUG_LOG_INFO("Logging module shutdown.");
}
#pragma endregion

#pragma region Container
bool Registry::register_logger(std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return register_logger_it(std::move(logger));
}

void Registry::register_or_replace_logger(std::shared_ptr<Logger> logger)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    register_or_replace_logger_it(std::move(logger));
}

void Registry::remove_logger(std::string_view name)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    bool isDefaultLogger = (_root != nullptr && _root->name() == name);
    _loggers.erase(name);
    if (isDefaultLogger) {
        _root.reset();
    }
}

void Registry::remove_all()
{
    {
        std::lock_guard<std::mutex> lock(_loggerMapMtx);
        if (_root != nullptr) {
            _root.reset();
        }
        _loggers.clear();
    }
    DEBUG_LOG_INFO("Remove all loggers.");
}

std::shared_ptr<Logger> Registry::get_logger(std::string_view name)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    auto it = _loggers.find(name);
    return it == _loggers.end() ? nullptr : it->second;
}

bool Registry::exist(std::string_view name)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return exist_it(name);
}

void Registry::register_task_pool(std::shared_ptr<TaskPool> taskPool)
{
    std::lock_guard<std::recursive_mutex> lock(_taskPoolMtx);
    _globalTaskPool = std::move(taskPool);
}

std::shared_ptr<TaskPool> Registry::get_task_pool()
{
    std::lock_guard<std::recursive_mutex> lock(_taskPoolMtx);
    return _globalTaskPool;
}
#pragma endregion

#pragma region private
bool Registry::register_logger_it(std::shared_ptr<Logger> logger)
{
    if (exist_it(logger->name())) {
        DEBUG_LOGGER_ERR("Logger already exist. LoggerName: {}.", logger->name());
        return false;
    }
    _loggers[logger->name()] = std::move(logger);
    return true;
}

void Registry::register_or_replace_logger_it(std::shared_ptr<Logger> logger)
{
    _loggers[logger->name()] = std::move(logger);
}

bool Registry::exist_it(std::string_view name)
{
    return (_loggers.find(name) != _loggers.end());
}
#pragma endregion
}  // namespace logging::details
