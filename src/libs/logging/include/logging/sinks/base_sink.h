#pragma once
#ifndef LOGGING_SINKS_BASE_SINK_H
#define LOGGING_SINKS_BASE_SINK_H

#include <string_view>

#include "logging/details/log_msg.h"
#include "logging/sinks/sink.h"

namespace logging {
class BaseSink : public Sink {
public:
    BaseSink() = default;
    ~BaseSink() override = default;

protected:
    void log_it(const details::LogMsg& logMsg) override;
    virtual void sink_it(std::string_view message) = 0;
    void flush_it() override = 0;
};
}  // namespace logging

#endif  // LOGGING_SINKS_BASE_SINK_H
