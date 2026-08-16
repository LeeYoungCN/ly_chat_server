#include "c/common_c.h"
#include "logging/c/logging_c.h"
#include "logging/formatters/pattern_formatter.h"

using namespace origin::logging;

extern "C" {
FormatterSt *logging_create_pattern_formatter(const char *pattern)
{
    if (pattern == nullptr) {
        return new struct FormatterSt(new PatternFormatter());
    } else {
        return new struct FormatterSt(new PatternFormatter(pattern));
    }
}

void logging_destroy_formatter(FormatterSt *formatter)
{
    if (formatter != nullptr) {
        if (formatter->ptr != nullptr) {
            formatter->ptr.reset();
        }
        delete formatter;
    }
}
}
