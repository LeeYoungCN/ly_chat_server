#include "logging/sinks/console_sink.h"

#include <cstdio>

#include "logging/logging.h"

namespace logging {
ConsoleSink::ConsoleSink(ConsoleType type) : m_type(type)
{
    if (m_type == CONSOLE_STDERR) {
        m_stream = stderr;
    } else {
        m_stream = stdout;
    }
}

void ConsoleSink::write(std::string_view message)
{
    fprintf(m_stream, "%s\n", message.data());
    std::fflush(m_stream);
}

void ConsoleSink::flush()
{
    std::fflush(m_stream);
}

}  // namespace logging
