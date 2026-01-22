#ifndef LOGGINGL_LOG_CONSOLE_SINK_H
#define LOGGINGL_LOG_CONSOLE_SINK_H

#include <cstdio>
#include <string_view>

#include "logging/sinks/sink.h"
#include "logging/logging.h"

namespace logging {
class ConsoleSink : public Sink {
public:
    ConsoleSink() = default;
    ~ConsoleSink() override = default;
    explicit ConsoleSink(ConsoleType type);
    void flush() override;
private:
    void write(std::string_view message) override;

private:
    ConsoleType m_type{ConsoleType::CONSOLE_STDOUT};
    FILE *m_stream = stdout;
};
}  // namespace logging
#endif  // LOGGINGL_LOG_CONSOLE_SINK_H
