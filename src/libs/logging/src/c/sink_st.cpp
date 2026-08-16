#include "c/common_c.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/stdout_sink.h"

using namespace origin::logging;

extern "C" {
SinkSt *origin_create_stdout_sink(FILE *file)
{
    return new struct SinkSt(std::make_shared<StdoutSink>(file));
}

SinkSt *origin_create_basic_file_sink(const char *file, bool overwrite)
{
    return new struct SinkSt(
        std::make_shared<BasicFileSink>((file == nullptr ? "" : file), overwrite));
}

void origin_detroy_sink(SinkSt *sink)
{
    if (sink != nullptr) {
        if (sink->ptr != nullptr) {
            sink->ptr.reset();
        }
        delete sink;
    }
}
}
