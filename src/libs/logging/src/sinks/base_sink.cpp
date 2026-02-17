#include "logging/sinks/base_sink.h"

#include <mutex>

#include "logging/formatters/formatter.h"

using namespace logging::details;

namespace logging {
void BaseSink::log(const LogMsg& logMsg)
{
    std::lock_guard lock(sink_mutex());
    std::string content;
    formatter()->format(logMsg, content);
    sink_it(content);
}

void BaseSink::flush()
{
    std::lock_guard lock(sink_mutex());
    flush_it();
}

}  // namespace logging
