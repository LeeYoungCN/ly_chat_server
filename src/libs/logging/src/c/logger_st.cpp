#include <memory>

#include "c/common_c.h"
#include "logging/c/logging_c.h"

using namespace origin::logging;
using namespace origin::logging::c;

extern "C" {

LoggerSt *origin_create_logger(const char *name, const SinkSt *const sinks[], uint32_t count)
{
    return new struct LoggerSt(std::make_shared<SyncLogger>(name, sink_ptr_vector(sinks, count)));
}
}
