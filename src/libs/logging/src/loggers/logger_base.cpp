#include "logging/loggers/logger_base.h"

#include <memory>
#include <string_view>
#include <vector>

#include "common/debug/debug_logger.h"
#include "loggers/internal/logger_impl.h"
#include "logging/formatters/formatter.h"
#include "logging/log_level.h"
#include "logging/log_source.h"
#include "logging/sinks/sink.h"

namespace logging {
LoggerBase::LoggerBase(std::shared_ptr<LoggerImpl> pImpl) : _pImpl(std::move(pImpl))
{
    throw_if_pimpl_null();
    DEBUG_LOGGER_DBG("Create logger. \"{}\".", _pImpl->name());
}

LoggerBase::~LoggerBase()
{
    if (_pImpl != nullptr) {
        DEBUG_LOGGER_DBG("Release logger. \"{}\".", _pImpl->name());
        _pImpl.reset();
    }
}

std::string_view LoggerBase::name() const
{
    throw_if_pimpl_null();
    return _pImpl->name();
}

std::vector<std::shared_ptr<Sink>> LoggerBase::sinks() const
{
    throw_if_pimpl_null();
    return _pImpl->sinks();
}

void LoggerBase::set_level(LogLevel level) const
{
    throw_if_pimpl_null();
    _pImpl->set_level(level);
}

LogLevel LoggerBase::level() const
{
    throw_if_pimpl_null();
    return _pImpl->level();
}
bool LoggerBase::should_log(LogLevel level) const
{
    throw_if_pimpl_null();
    return _pImpl->should_log(level);
}

void LoggerBase::flush_on(LogLevel level) const
{
    throw_if_pimpl_null();
    _pImpl->flush_on(level);
}

LogLevel LoggerBase::flush_level() const
{
    throw_if_pimpl_null();
    return _pImpl->flush_level();
}

bool LoggerBase::should_flush(LogLevel level) const
{
    throw_if_pimpl_null();
    return _pImpl->should_flush(level);
}

void LoggerBase::set_pattern(std::string_view pattern) const
{
    throw_if_pimpl_null();
    _pImpl->set_pattern(pattern);
}

void LoggerBase::set_formatter(const std::unique_ptr<Formatter>& formatter) const
{
    throw_if_pimpl_null();
    _pImpl->set_formatter(formatter);
}

void LoggerBase::flush()
{
    throw_if_pimpl_null();
    _pImpl->flush();
}

void LoggerBase::log_it(const LogSource& source, LogLevel level, std::string_view message)
{
    throw_if_pimpl_null();
    _pImpl->log(source, level, message);
}

void LoggerBase::throw_if_pimpl_null() const
{
    if (_pImpl == nullptr) {
        throw std::runtime_error("pImpl nullptr.");
    }
}

}  // namespace logging
