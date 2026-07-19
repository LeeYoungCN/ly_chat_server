#pragma once
#ifndef LOGGING_SINKS_STDOUT_SINK_H
#define LOGGING_SINKS_STDOUT_SINK_H

#include <cstdio>

#include "logging/sinks/base_sink.h"

namespace logging {
class StdoutSink : public BaseSink {
public:
    StdoutSink();
    ~StdoutSink() override = default;
    explicit StdoutSink(FILE *file);
};
}  // namespace logging
#endif  // LOGGING_SINKS_STDOUT_SINK_H
