#include "logging/sinks/stdout_sink.h"

#include <cstdio>

namespace logging {
StdoutSink::StdoutSink(FILE *file) : m_stream(file == nullptr ? stdout : file) {}

void StdoutSink::sink_it(std::string_view message)
{
    fprintf(m_stream, "%s\n", message.data());
    std::fflush(m_stream);
}

void StdoutSink::flush_it()
{
    std::fflush(m_stream);
}

}  // namespace logging
