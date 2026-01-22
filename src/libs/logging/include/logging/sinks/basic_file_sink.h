#ifndef LOGGINGL_LOG_BASIC_SINK_H
#define LOGGINGL_LOG_BASIC_SINK_H

#include <string_view>

#include "common/utils/file_writer.h"
#include "logging/sinks/sink.h"

namespace logging {
class BasicFileSink : public Sink {
public:
    BasicFileSink();
    ~BasicFileSink() override;
    explicit BasicFileSink(std::string_view file, bool overwrite = true);

    void flush() override;

protected:
    void write(std::string_view message) override;

private:
    std::string m_file;
    bool m_overwrite{true};
    common::utils::filesystem::FileWriter m_fileWriter;
};
}  // namespace logging

#endif  // LOGGINGL_LOG_BASIC_SINK_H
