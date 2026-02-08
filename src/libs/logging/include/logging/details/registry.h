#pragma once

#ifndef LOGGING_DETAILS_REGISTRY_H
#define LOGGING_DETAILS_REGISTRY_H

#include <memory>
#include <mutex>
#include <string_view>
#include <unordered_map>

#include "common/base/singleton.h"
#include "logging/details/common.h"
#include "logging/details/log_thread_pool.h"
#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/logger.h"

namespace logging::details {
class Registry : public common::base::SingletonBase<Registry> {
    friend common::base::SingletonBase<Registry>;

public:
#pragma region root
    std::shared_ptr<Logger> root_logger();
    Logger* root_logger_raw();
    void set_root_logger(std::shared_ptr<Logger> newLogger);
#pragma endregion root

#pragma region control all
    void initialize_logger(const std::shared_ptr<Logger>& logger, bool autoRegister = true);
    void set_level_all(LogLevel level);
    void flush_on_all(LogLevel level);
    void set_pattern_all(std::string_view pattern = FORMATTER_DEFAULT_PATTERN,
                         std::string_view timePattern = FORMATTER_DEFAULT_TIME_PATTERN);
    void set_formatter_all(std::unique_ptr<Formatter> formatter);
    void set_thread_pool(std::shared_ptr<LogThreadPool> threadPool);
    std::shared_ptr<LogThreadPool> get_thread_pool();
    void flush_all();
    void shut_down();
#pragma endregion control all

#pragma region container
    bool exist(std::string_view loggerName);
    bool register_logger(std::shared_ptr<Logger> logger);
    void register_or_replace_logger(std::shared_ptr<Logger> logger);
    void remove_logger(std::string_view name);
    void remove_all();
    std::shared_ptr<Logger> get(std::string_view loggerName);
#pragma endregion container

private:
    bool register_logger_it(std::shared_ptr<Logger> logger);
    void register_or_replace_logger_it(std::shared_ptr<Logger> logger);
    bool exist_it(std::string_view name);

protected:
    Registry();
    ~Registry() override = default;

private:
    // root logger
    std::shared_ptr<Logger> _root;

    // container
    std::unordered_map<std::string_view, std::shared_ptr<Logger>> _loggers;
    std::mutex _loggerMapMtx;

    // global options
    LogLevel _globalLevel{LogLevel::INFO};
    LogLevel _globalFlushLevel{LogLevel::OFF};
    std::unique_ptr<Formatter> _globalFormatter;
    std::recursive_mutex _threadPoolMtx;
    std::shared_ptr<LogThreadPool> _globalThreadPool;
};
}  // namespace logging::details

#endif  // LOGGING_DETAILS_REGISTRY_H
