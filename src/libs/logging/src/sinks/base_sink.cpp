#include "logging/sinks/base_sink.h"

#include <mutex>

#include "logging/formatters/formatter.h"

using namespace logging::details;

namespace logging {
void BaseSink::log(const LogMsg& logMsg)
{
    std::lock_guard lock(_sinkMtx);
    std::string content;
    _formatter->format(logMsg, content);
    sink_it(content);
}

void BaseSink::flush()
{
    std::lock_guard lock(_sinkMtx);
    flush_it();
}

}  // namespace logging
