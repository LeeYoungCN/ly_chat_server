#include "logging/sinks/sink.h"

namespace logging {
void Sink::log(const details::LogMsg& logMsg)
{
    constexpr uint32_t LOG_CONTENT_DEFAULT_LEN = 256;
    std::string content;
    content.reserve(LOG_CONTENT_DEFAULT_LEN);
    _formatter->format(logMsg, content);
    write(content);
}

}  // namespace logging
