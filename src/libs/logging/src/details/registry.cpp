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
    _root = std::make_shared<Logger>(DEFAULT_LOGGER_NAME, std::make_shared<StdoutSink>());
    _loggers[DEFAULT_LOGGER_NAME] = _root;
}

#pragma region root
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
#pragma endregion root

#pragma region control all
void Registry::initialize_logger(const std::shared_ptr<Logger>& logger, bool autoRegister)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    logger->sef_formatter(_globalFormatter->clone());
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

void Registry::set_pattern_all(const std::string& pattern, const std::string& timePattern)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    set_formatter_all(std::make_unique<PatternFormatter>(pattern, timePattern));
}

void Registry::set_formatter_all(std::unique_ptr<Formatter> formatter)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    _globalFormatter = std::move(formatter);
    for (auto& [_, logger] : _loggers) {
        logger->sef_formatter(_globalFormatter->clone());
    }
}

void Registry::set_thread_pool(std::shared_ptr<LogThreadPool> threadPool)
{
    std::lock_guard<std::recursive_mutex> lock(_threadPoolMtx);
    _globalThreadPool = std::move(threadPool);
}

std::shared_ptr<LogThreadPool> Registry::get_thread_pool()
{
    std::lock_guard<std::recursive_mutex> lock(_threadPoolMtx);
    return _globalThreadPool;
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
    {
        std::lock_guard<std::recursive_mutex> lock(_threadPoolMtx);
        if (_globalThreadPool != nullptr) {
            _globalThreadPool.reset();
        }
    }
    remove_all();
    DEBUG_LOG_INFO("Logging module shutdown.");
}
#pragma endregion control all

#pragma region container
bool Registry::exist(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    return exist_it(name);
}

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


void Registry::remove_logger(const std::string& name)
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

std::shared_ptr<Logger> Registry::get(const std::string& name)
{
    std::lock_guard<std::mutex> lock(_loggerMapMtx);
    auto it = _loggers.find(name);
    return it == _loggers.end() ? nullptr : it->second;
}
#pragma endregion container

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

bool Registry::exist_it(const std::string& name)
{
    return (_loggers.find(name) != _loggers.end());
}
#pragma endregion
}  // namespace logging::details
