#include "logging/sinks/stdout_sink.h"

#include <cstdio>

namespace logging {
StdoutSink::StdoutSink(FILE *file) : m_stream(file == nullptr ? stdout : file)
{}

void StdoutSink::write(std::string_view message)
{
    fprintf(m_stream, "%s\n", message.data());
    std::fflush(m_stream);
}

void StdoutSink::flush()
{
    std::fflush(m_stream);
}

}  // namespace logging
