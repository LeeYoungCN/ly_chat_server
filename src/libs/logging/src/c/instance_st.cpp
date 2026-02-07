#include "logging/logger.h"
#include "logging/logging_c.h"
#include "logging/sinks/basic_file_sink.h"
#include "logging/sinks/stdout_sink.h"

using namespace logging;

struct sink_st {
    std::shared_ptr<logging::Sink> ptr;

    explicit sink_st(std::shared_ptr<logging::Sink> sink) : ptr(std::move(sink)) {}
    explicit sink_st(logging::Sink *sink) : ptr(sink) {}
};

struct logger_st {
    std::shared_ptr<logging::Logger> ptr;

    explicit logger_st(std::shared_ptr<logging::Logger> ptr) : ptr(std::move(ptr)) {}
    explicit logger_st(logging::Logger *ptr) : ptr(ptr) {}
};

extern "C" {

sink_st *logging_stdout_sink(FILE *file)
{
    return new sink_st(std::make_shared<StdoutSink>(file));
}

sink_st *logging_basic_file_sink(const char *file, bool overwrite)
{
    return new sink_st(std::make_shared<BasicFileSink>((file == nullptr ? "" : file), overwrite));
}

logger_st *logging_logger(const char *name, sink_st *sinks[], uint32_t count)
{
    std::vector<std::shared_ptr<Sink>> sinkList(count);
    for (uint32_t i = 0; i < count; i++) {
        sinkList[i] = std::move(sinks[i]->ptr);
        delete sinks[i];
    }
    return new logger_st(new Logger(name, sinkList));
}
}
