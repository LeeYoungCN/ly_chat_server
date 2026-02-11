#include "c/common_c.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/stdout_sink.h"

using namespace logging;

extern "C" {
SinkSt *logging_create_stdout_sink(FILE *file)
{
    return new SinkSt(std::make_shared<StdoutSink>(file));
}

SinkSt *logging_create_basic_file_sink(const char *file, bool overwrite)
{
    return new SinkSt(std::make_shared<BasicFileSink>((file == nullptr ? "" : file), overwrite));
}

void logging_detroy_sink(SinkSt *sink)
{
    if (sink == nullptr) {
        return;
    }
    sink->ptr.reset();
    delete sink;
}
}
