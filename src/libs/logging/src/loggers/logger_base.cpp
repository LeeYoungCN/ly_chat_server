#include "logging/loggers/logger_base.h"

#include <memory>
#include <string_view>
#include <vector>

#include "internal/loggers/logger_impl.h"
#include "logging/details/log_source.h"
#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/sinks/sink.h"

namespace logging {
LoggerBase::LoggerBase(std::shared_ptr<LoggerImpl> pImpl) : _pimpl(std::move(pImpl))
{
    throw_if_pimpl_null();
}

LoggerBase::~LoggerBase() = default;

std::string_view LoggerBase::name() const
{
    throw_if_pimpl_null();
    return _pimpl->name();
}

std::vector<std::shared_ptr<logging::Sink>> LoggerBase::sinks() const
{
    throw_if_pimpl_null();
    return _pimpl->sinks();
}

void LoggerBase::set_level(LogLevel level) const
{
    throw_if_pimpl_null();
    _pimpl->set_level(level);
}

LogLevel LoggerBase::level() const
{
    throw_if_pimpl_null();
    return _pimpl->level();
}
bool LoggerBase::should_log(LogLevel level) const
{
    throw_if_pimpl_null();
    return _pimpl->should_log(level);
}

void LoggerBase::flush_on(LogLevel level) const
{
    throw_if_pimpl_null();
    _pimpl->flush_on(level);
}

LogLevel LoggerBase::flush_level() const
{
    throw_if_pimpl_null();
    return _pimpl->flush_level();
}

bool LoggerBase::should_flush(LogLevel level) const
{
    throw_if_pimpl_null();
    return _pimpl->should_flush(level);
}

void LoggerBase::set_pattern(std::string_view pattern) const
{
    throw_if_pimpl_null();
    _pimpl->set_pattern(pattern);
}

void LoggerBase::set_formatter(const std::unique_ptr<logging::Formatter>& formatter) const
{
    throw_if_pimpl_null();
    _pimpl->set_formatter(formatter);
}

void LoggerBase::flush()
{
    throw_if_pimpl_null();
    _pimpl->flush();
}

void LoggerBase::log_it(const details::LogSource& source, LogLevel level, std::string_view message)
{
    throw_if_pimpl_null();
    _pimpl->log(source, level, message);
}

void LoggerBase::throw_if_pimpl_null() const
{
    if (_pimpl == nullptr) {
        throw std::runtime_error("pImpl nullptr.");
    }
}

}  // namespace logging
