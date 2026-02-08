#include "logging/logging.h"

#include <utility>

#include "common/base/singleton.h"
#include "logging/details/registry.h"

namespace logging {
// global options
void initialize_logger(const std::shared_ptr<Logger>& logger)
{
    INST(details::Registry).initialize_logger(logger);
}

void set_level_all(LogLevel level)
{
    INST(details::Registry).set_level_all(level);
}

void flush_on_all(LogLevel level)
{
    INST(details::Registry).flush_on_all(level);
}

void set_pattern_all(std::string_view pattern, std::string_view timePattern)
{
    INST(details::Registry).set_pattern_all(pattern, timePattern);
}

void set_formatter_all(std::unique_ptr<Formatter> formatter)
{
    INST(details::Registry).set_formatter_all(std::move(formatter));
}

void set_thread_pool(std::shared_ptr<details::LogThreadPool> threadPool)
{
    INST(details::Registry).set_thread_pool(std::move(threadPool));
}

std::shared_ptr<details::LogThreadPool> get_thread_pool()
{
    return INST(details::Registry).get_thread_pool();
}

void flush_all()
{
    INST(details::Registry).flush_all();
}

void shut_down()
{
    INST(details::Registry).shut_down();
}

// container
bool register_logger(std::shared_ptr<Logger> logger)
{
    return INST(details::Registry).register_logger(std::move(logger));
}

void register_or_replace_logger(std::shared_ptr<Logger> logger)
{
    INST(details::Registry).register_or_replace_logger(std::move(logger));
}

void remove_logger(std::string_view name)
{
    INST(details::Registry).remove_logger(name);
}

void remove_all()
{
    INST(details::Registry).remove_all();
}

std::shared_ptr<Logger> root_logger()
{
    return INST(details::Registry).root_logger();
}

Logger* root_logger_raw()
{
    return INST(details::Registry).root_logger_raw();
}

void set_root_logger(std::shared_ptr<Logger> logger)
{
    INST(details::Registry).set_root_logger(std::move(logger));
}

bool should_log(LogLevel level)
{
    return root_logger_raw()->should_log(level);
}

void set_level(LogLevel level)
{
    root_logger_raw()->set_level(level);
}

void flush_on(LogLevel level)
{
    root_logger_raw()->flush_on(level);
}

void set_pattern(std::string_view pattern, std::string_view timePattern)
{
    root_logger_raw()->set_pattern(pattern, timePattern);
}

void set_formatter(std::unique_ptr<Formatter> formatter)
{
    root_logger_raw()->sef_formatter(std::move(formatter));
}

void flush()
{
    root_logger_raw()->flush();
}

}  // namespace logging
