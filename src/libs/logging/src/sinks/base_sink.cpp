#include "logging/sinks/base_sink.h"

#include <stdexcept>
#include <utility>

namespace logging {
BaseSink::~BaseSink() = default;

BaseSink::BaseSink(std::unique_ptr<Sink> pImpl) : _pImpl(std::move(pImpl))
{
    throw_if_pimpl_null();
}

void BaseSink::log(const details::LogMsg& logMsg)
{
    throw_if_pimpl_null();
    _pImpl->log(logMsg);
}

void BaseSink::flush()
{
    throw_if_pimpl_null();
    _pImpl->flush();
}

void BaseSink::set_pattern(std::string_view pattern)
{
    throw_if_pimpl_null();
    _pImpl->set_pattern(pattern);
}

void BaseSink::set_formatter(std::unique_ptr<Formatter> formatter)
{
    throw_if_pimpl_null();
    _pImpl->set_formatter(std::move(formatter));
}

bool BaseSink::should_log(LogLevel level) const
{
    throw_if_pimpl_null();
    return _pImpl->should_log(level);
}

void BaseSink::set_level(LogLevel level)
{
    throw_if_pimpl_null();
    return _pImpl->set_level(level);
}

LogLevel BaseSink::level() const
{
    throw_if_pimpl_null();
    return _pImpl->level();
}

void BaseSink::throw_if_pimpl_null() const
{
    if (_pImpl == nullptr) {
        throw std::runtime_error("pImpl nullptr.");
    }
}

}  // namespace logging
