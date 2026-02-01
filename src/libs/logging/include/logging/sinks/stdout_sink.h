#ifndef LOGGINGL_LOG_STDOUT_SINK_H
#define LOGGINGL_LOG_STDOUT_SINK_H

#include <cstdio>
#include <string_view>

#include "logging/sinks/sink.h"

namespace logging {
class StdoutSink : public Sink {
public:
    StdoutSink() = default;
    ~StdoutSink() override = default;
    explicit StdoutSink(FILE *file);
    void flush() override;
private:
    void write(std::string_view message) override;

private:
    FILE *m_stream = stdout;
};
}  // namespace logging
#endif  // LOGGINGL_LOG_STDOUT_SINK_H
