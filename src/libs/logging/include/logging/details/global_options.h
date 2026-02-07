
#ifndef LOGGING_DETAILS_GLOBAL_CONFIG_H
#define LOGGING_DETAILS_GLOBAL_CONFIG_H
#include <memory>
#include <utility>

#include "common/base/singleton.h"
#include "logging/details/log_thread_pool.h"
#include "logging/formatters/formatter.h"
#include "logging/formatters/pattern_formatter.h"
#include "logging/log_level.h"

namespace logging::details {

class GlobalOptions : public common::base::SingletonBase<GlobalOptions> {
    friend common::base::SingletonBase<GlobalOptions>;

public:
    std::unique_ptr<Formatter> get_formatter() { return _formatter->clone(); }
    void set_formatter(std::unique_ptr<Formatter> formatter) { _formatter = std::move(formatter); }

    void set_level(LogLevel level) { _level = level; }
    [[nodiscard]] LogLevel level() const { return _level; }

    void flush_on(LogLevel level) { _flushLevel = level; }
    [[nodiscard]] LogLevel flush_level() const { return _flushLevel; }

    void set_thread_pool(std::shared_ptr<LogThreadPool> pool)
    {
        _pool = std::move(pool);
    }

    [[nodiscard]] std::shared_ptr<LogThreadPool> thread_pool() const { return _pool; }

private:
    LogLevel _level{LogLevel::INFO};
    LogLevel _flushLevel{LogLevel::OFF};
    std::unique_ptr<Formatter> _formatter{new PatternFormatter()};
    std::shared_ptr<LogThreadPool> _pool;
};
}  // namespace logging::details
#endif  // LOGGING_DETAILS_GLOBAL_CONFIG_H
