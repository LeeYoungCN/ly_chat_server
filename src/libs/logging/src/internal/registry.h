#pragma once

#ifndef LOGGING_DETAILS_REGISTRY_H
#define LOGGING_DETAILS_REGISTRY_H

#include <memory>
#include <mutex>
#include <string_view>
#include <unordered_map>

#include "common/base/singleton.h"
#include "internal/task_pool.h"
#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/loggers/logger.h"

#define REGISTRY (Registry::instance())

namespace logging {
class Registry : public common::base::SingletonBase<Registry> {
    friend common::base::SingletonBase<Registry>;

public:
#pragma region Root logger
    std::shared_ptr<Logger> root_logger();
    Logger* root_logger_raw();
    void set_root_logger(std::shared_ptr<Logger> newLogger);
#pragma endregion root

#pragma region Module manager
    void initialize_logger(const std::shared_ptr<Logger>& logger, bool autoRegister = true);
    void set_level_all(LogLevel level);
    void flush_on_all(LogLevel level);
    void set_pattern_all(std::string_view pattern);
    void set_formatter_all(std::unique_ptr<Formatter> formatter);
    void flush_all();
    void shutdown();
#pragma endregion

#pragma region Logger container
    bool register_logger(std::shared_ptr<Logger> logger);
    void register_or_replace_logger(std::shared_ptr<Logger> logger);
    void remove_logger(std::string_view name);
    void remove_all();
    std::shared_ptr<Logger> get_logger(std::string_view loggerName);
    bool exist(std::string_view loggerName);
#pragma endregion

#pragma region Task pool container
    void init_root_task_pool(uint32_t capacity, uint32_t threadCnt);
    std::shared_ptr<TaskPool> task_pool();
#pragma endregion

private:
    bool register_logger_it(std::shared_ptr<Logger> logger);
    void register_or_replace_logger_it(std::shared_ptr<Logger> logger);
    bool exist_it(std::string_view name);

protected:
    Registry();
    ~Registry() override = default;

private:
    // root logger
    std::shared_ptr<Logger> _rootLogger;

    // container
    std::unordered_map<std::string_view, std::shared_ptr<Logger>> _loggers;
    std::mutex _loggerMapMtx;

    // global options
    LogLevel _globalLevel{LogLevel::INFO};
    LogLevel _globalFlushLevel{LogLevel::OFF};
    std::unique_ptr<Formatter> _globalFormatter;
    std::recursive_mutex _taskPoolMtx;
    std::shared_ptr<TaskPool> _rootTaskPool;
};
}  // namespace logging

#endif  // LOGGING_DETAILS_REGISTRY_H
