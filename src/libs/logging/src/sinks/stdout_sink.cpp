#include "logging/sinks/stdout_sink.h"

#include <cstdio>
#include <memory>

#include "internal/sinks/stdout_sink_impl.h"
#include "logging/sinks/sink_base.h"

namespace logging {
StdoutSink::StdoutSink() : SinkBase(std::make_unique<StdoutSinkImpl>()) {}

StdoutSink::StdoutSink(FILE *file) : SinkBase(std::make_unique<StdoutSinkImpl>(file)) {}

}  // namespace logging
