#pragma once
#ifndef LOGGING_INTERNAL_SINKS_STDOUT_SINK_IMPL_H
#define LOGGING_INTERNAL_SINKS_STDOUT_SINK_IMPL_H

#include <cstdio>
#include <string_view>

#include "internal/sinks/sink_impl_base.h"

namespace logging {
class StdoutSinkImpl : public SinkImplBase {
public:
    StdoutSinkImpl();
    ~StdoutSinkImpl() override = default;
    explicit StdoutSinkImpl(FILE *file);

private:
    void log_it(const details::LogMsg &logMsg) override;
    void flush_it() override;
    void sink_it(std::string_view message);

private:
    FILE *m_stream = stdout;
};
}  // namespace logging
#endif  // LOGGING_INTERNAL_SINKS_STDOUT_SINK_IMPL_H
