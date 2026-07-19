#include "logging/formatters/formatter.h"

#include <memory>
#include <stdexcept>

#include "internal/formatters/formatter_impl.h"

namespace logging {
Formatter::~Formatter() = default;

Formatter::Formatter(std::unique_ptr<FormatterImpl> pimpl) : _pImpl(std::move(pimpl))
{
    if (_pImpl == nullptr) {
        throw std::runtime_error("pimpl nullptr");
    }
}

void Formatter::format(const details::LogMsg& logMsg, std::string& content)
{
    if (_pImpl == nullptr) {
        throw std::runtime_error("pimpl nullptr");
    }
    _pImpl->format(logMsg, content);
}

std::unique_ptr<Formatter> Formatter::clone() const
{
    if (_pImpl == nullptr) {
        throw std::runtime_error("pimpl nullptr");
    }
    return std::make_unique<Formatter>(_pImpl->clone());
}

}  // namespace logging
