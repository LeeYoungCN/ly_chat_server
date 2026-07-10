#pragma once
#ifndef LOGGINGL_LOG_BASIC_SINK_H
#define LOGGINGL_LOG_BASIC_SINK_H

#include <string>
#include <string_view>

#include "logging/sinks/sink.h"

namespace logging {
class BasicFileSink : public Sink {
public:
    BasicFileSink();
    ~BasicFileSink() override;
    explicit BasicFileSink(std::string_view file, bool overwrite = true);

public:
    void log(const logging::details::LogMsg& logMsg) override;
    void flush() override;

    std::string file();

protected:
    virtual void flush_it();
    virtual void sink_it(std::string_view message);

private:
    struct Impl;
    std::unique_ptr<Impl> _pimpl;
};

}  // namespace logging

#endif  // LOGGINGL_LOG_BASIC_SINK_H
