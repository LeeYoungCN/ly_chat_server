#include "logging/sinks/base_sink.h"

#include "logging/formatters/formatter.h"

using namespace logging::details;

namespace logging {
void BaseSink::log_it(const details::LogMsg& logMsg)
{
    std::string content;
    formatter()->format(logMsg, content);
    sink_it(content);
}

}  // namespace logging
