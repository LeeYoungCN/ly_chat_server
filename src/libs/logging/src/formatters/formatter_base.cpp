#include "logging/formatters/formatter_base.h"

#include <memory>
#include <stdexcept>

#include "logging/formatters/formatter.h"

namespace logging {
FormatterBase::~FormatterBase() = default;

FormatterBase::FormatterBase(std::unique_ptr<Formatter> pimpl) : _pImpl(std::move(pimpl))
{
    throw_if_pimpl_null();
}

void FormatterBase::format(const LogMsg& logMsg, std::string& content)
{
    throw_if_pimpl_null();
    _pImpl->format(logMsg, content);
}

std::unique_ptr<Formatter> FormatterBase::clone() const
{
    throw_if_pimpl_null();
    return std::make_unique<FormatterBase>(_pImpl->clone());
}

void FormatterBase::throw_if_pimpl_null() const
{
    if (_pImpl == nullptr) {
        throw std::runtime_error("pImpl nullptr.");
    }
}

}  // namespace logging
