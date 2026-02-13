#pragma once
#ifndef LOGGING_SINKS_BASE_SINK_H
#define LOGGING_SINKS_BASE_SINK_H

#include <string_view>

#include "common/compiler/macros.h"
#include "logging/details/log_msg.h"
#include "logging/sinks/sink.h"

namespace logging {
class COMMON_API BaseSink : public Sink {
public:
    BaseSink() = default;
    ~BaseSink() override = default;

    void log(const logging::details::LogMsg& logMsg) override;
    void flush() override;

protected:
    virtual void sink_it(std::string_view message) = 0;
    virtual void flush_it() = 0;
};
}  // namespace logging

#endif  // LOGGING_SINKS_BASE_SINK_H
