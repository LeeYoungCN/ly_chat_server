#pragma once

#ifndef LOGGING_DETAILS_REGISTRY_H
#define LOGGING_DETAILS_REGISTRY_H

#include <memory>
#include <string>
#include <unordered_map>

#include "common/base/singleton.h"
#include "logging/logger.h"

namespace logging::details {
class Registry : public common::base::SingletonBase<Registry> {
    friend common::base::SingletonBase<Registry>;

public:
    // default logger
    std::shared_ptr<Logger> default_logger();
    Logger* default_logger_raw();
    void set_default_logger(std::shared_ptr<Logger> newLogger);

    // container
    bool exist(const std::string& name);

    bool register_logger(std::shared_ptr<Logger> logger);
    bool register_logger(const std::string& name, std::shared_ptr<Logger> logger);

    void register_or_replace_logger(std::shared_ptr<Logger> logger);
    void register_or_replace_logger(const std::string& name, std::shared_ptr<Logger> logger);

    void remove_logger(const std::string& name);
    void remove_all_logger();

    std::shared_ptr<Logger> get(const std::string& name);

private:
    bool register_logger_it(const std::string& name, std::shared_ptr<Logger> logger);
    bool exist_it(const std::string& name);

protected:
    Registry();
    ~Registry() override = default;

private:
    // default logger
    std::shared_ptr<Logger> _defaultLogger;
    // container
    std::unordered_map<std::string, std::shared_ptr<Logger>> _loggers;
    std::mutex _loggerMapMtx;
};
}  // namespace logging::details

#endif  // LOGGING_DETAILS_REGISTRY_H
