#include "logging/formatters/basic_formatter.h"

#include <memory>
#include <stdexcept>

#include "logging/formatters/formatter.h"

namespace logging {
BasicFormatter::~BasicFormatter() = default;

BasicFormatter::BasicFormatter(std::unique_ptr<Formatter> pimpl) : _pImpl(std::move(pimpl))
{
    throw_if_pimpl_null();
}

void BasicFormatter::format(const details::LogMsg& logMsg, std::string& content)
{
    throw_if_pimpl_null();
    _pImpl->format(logMsg, content);
}

std::unique_ptr<Formatter> BasicFormatter::clone() const
{
    throw_if_pimpl_null();
    return std::make_unique<BasicFormatter>(_pImpl->clone());
}

void BasicFormatter::throw_if_pimpl_null() const
{
    if (_pImpl == nullptr) {
        throw std::runtime_error("pImpl nullptr.");
    }
}

}  // namespace logging
