#ifndef LOGGINGL_LOG_BASIC_SINK_H
#define LOGGINGL_LOG_BASIC_SINK_H

#include <string_view>

#include "common/utils/file_writer.h"
#include "logging/log_sink.h"

namespace logging {
class LogBasicFileSink : public LogSink {
public:
    LogBasicFileSink() = default;
    ~LogBasicFileSink() override = default;
    LogBasicFileSink(std::string_view file, bool overwrite);

    void write(std::string message) override;

protected:
    void flush() override;

private:
    std::string m_file;
    bool m_overwrite{true};
    common::utils::filesystem::FileWriter m_fileAppender;
};
}  // namespace logging

#endif  // LOGGINGL_LOG_BASIC_SINK_H
