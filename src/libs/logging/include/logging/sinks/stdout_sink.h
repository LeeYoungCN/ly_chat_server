#pragma once
#ifndef LOGGING_SINKS_STDOUT_SINK_H
#define LOGGING_SINKS_STDOUT_SINK_H

#include <cstdio>

#include "logging/sinks/sink_base.h"

namespace origin::logging {
class StdoutSink : public SinkBase {
public:
    StdoutSink();
    ~StdoutSink() override = default;
    explicit StdoutSink(FILE *file);
};
}  // namespace origin::logging
#endif  // LOGGING_SINKS_STDOUT_SINK_H
