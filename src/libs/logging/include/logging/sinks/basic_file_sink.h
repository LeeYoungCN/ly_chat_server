#pragma once
#ifndef LOGGINGL_LOG_BASIC_SINK_H
#define LOGGINGL_LOG_BASIC_SINK_H

#include <string_view>

#include "common/compiler/macros.h"
#include "logging/sinks/base_sink.h"


namespace logging {
class COMMON_API BasicFileSink : public BaseSink {
public:
    BasicFileSink();
    ~BasicFileSink() override;
    explicit BasicFileSink(std::string_view file, bool overwrite = true);

protected:
    void flush_it() override;
    void sink_it(std::string_view message) override;

private:
    struct Impl;
    Impl* _pimpl;
};

}  // namespace logging

#endif  // LOGGINGL_LOG_BASIC_SINK_H
