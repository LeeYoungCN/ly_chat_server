#pragma once
#ifndef LOGGINGL_LOG_STDOUT_SINK_H
#define LOGGINGL_LOG_STDOUT_SINK_H

#include <cstdio>
#include <string_view>

#include "logging/sinks/base_sink.h"

namespace logging {
class StdoutSink : public BaseSink {
public:
    StdoutSink() = default;
    ~StdoutSink() override = default;
    explicit StdoutSink(FILE *file);

private:
    void flush_it() override;
    void sink_it(std::string_view message) override;

private:
    FILE *m_stream = stdout;
};
}  // namespace logging
#endif  // LOGGINGL_LOG_STDOUT_SINK_H
