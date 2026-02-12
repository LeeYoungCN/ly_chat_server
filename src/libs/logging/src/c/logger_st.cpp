#include <memory>

#include "c/common_c.h"
#include "logging/c/logging_c.h"

using namespace logging;
using namespace logging::c;

extern "C" {

LoggerSt *logging_create_logger(const char *name, const SinkSt *const sinks[], uint32_t count)
{
    return new LoggerSt(std::make_shared<Logger>(name, sink_ptr_vector(sinks, count)));
}
}
