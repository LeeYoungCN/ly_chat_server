#include "logging/sinks/stdout_sink.h"

#include <cstdio>
#include <string>

namespace logging {
StdoutSink::StdoutSink() : StdoutSink(stdout) {}

StdoutSink::StdoutSink(FILE *file) : m_stream(file == nullptr ? stdout : file)
{
    if (m_stream == stdout) {
        _paramStr = std::string("StdoutSink, stream: stdout.");
    } else if (m_stream == stderr) {
        _paramStr = std::string("StdoutSink, stream: stderr.");
    } else {
        _paramStr = std::string("StdoutSink, stream: " +
                                std::to_string(reinterpret_cast<std::uintptr_t>(m_stream)) + ".");
    }
}

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
