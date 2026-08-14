#include "logging/formatters/pattern_formatter.h"

#include <memory>

#include "formatters/internal/pattern_formatter_impl.h"

namespace logging {

PatternFormatter::PatternFormatter() : FormatterBase(std::make_unique<PatternFormatterImpl>()) {}

PatternFormatter::PatternFormatter(std::string_view pattern)
    : FormatterBase(std::make_unique<PatternFormatterImpl>(pattern))
{
}

}  // namespace logging
