#include "c/common_c.h"
#include "logging/c/logging_c.h"
#include "logging/details/constants.h"
#include "logging/formatters/pattern_formatter.h"

using namespace logging;

extern "C" {
FormatterSt *logging_create_pattern_formatter(const char *pattern)
{
    return new FormatterSt(new logging::PatternFormatter(
        (pattern == nullptr ? logging::details::FORMATTER_DEFAULT_PATTERN : pattern)));
}

void logging_destroy_formatter(FormatterSt *formatter)
{
    if (formatter == nullptr) {
        return;
    }
    formatter->ptr.reset();
    delete formatter;
}
}
