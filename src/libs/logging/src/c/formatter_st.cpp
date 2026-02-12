#include "c/common_c.h"
#include "logging/c/logging_c.h"
#include "logging/details/common.h"
#include "logging/formatters/pattern_formatter.h"

using namespace logging;

extern "C" {
FormatterSt *logging_create_pattern_formatter(const char *pattern, const char *timePattern)
{
    return new FormatterSt(new logging::PatternFormatter(
        (pattern == nullptr ? FORMATTER_DEFAULT_PATTERN : pattern),
        (timePattern == nullptr ? FORMATTER_DEFAULT_TIME_PATTERN : timePattern)));
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
