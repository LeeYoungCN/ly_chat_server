#include "logging/formatters/pattern_formatter.h"

#include <memory>

#include "internal/formatters/pattern_formatter_impl.h"

namespace logging {

PatternFormatter::PatternFormatter() : BasicFormatter(std::make_unique<PatternFormatterImpl>()) {}

PatternFormatter::PatternFormatter(std::string_view pattern)
    : BasicFormatter(std::make_unique<PatternFormatterImpl>(pattern))
{
}

}  // namespace logging
