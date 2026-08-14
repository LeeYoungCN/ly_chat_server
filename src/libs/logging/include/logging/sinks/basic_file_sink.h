#pragma once
#ifndef LOGGING_SINKS_BASIC_FILE_SINK_H
#define LOGGING_SINKS_BASIC_FILE_SINK_H

#include <string>
#include <string_view>

#include "logging/sinks/sink_base.h"

namespace logging {
class BasicFileSink : public SinkBase {
public:
    BasicFileSink();
    ~BasicFileSink() override = default;
    explicit BasicFileSink(std::string_view file, bool overwrite = true);

    [[nodiscard]] std::string file() const;
};

}  // namespace logging

#endif  // LOGGING_SINKS_BASIC_FILE_SINK_H
