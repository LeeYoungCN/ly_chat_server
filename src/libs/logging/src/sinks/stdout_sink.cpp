#include "logging/sinks/stdout_sink.h"

#include <cstdio>
#include <memory>

#include "internal/sinks/stdout_sink_impl.h"
#include "logging/sinks/base_sink.h"

namespace logging {
StdoutSink::StdoutSink() : BaseSink(std::make_unique<StdoutSinkImpl>()) {}

StdoutSink::StdoutSink(FILE *file) : BaseSink(std::make_unique<StdoutSinkImpl>(file)) {}

}  // namespace logging
