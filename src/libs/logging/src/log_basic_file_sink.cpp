#include "logging/log_basic_file_sink.h"

#include "common/utils/file_writer.h"

namespace logging {
using namespace common::utils::filesystem;

LogBasicFileSink::LogBasicFileSink(std::string_view file, bool overwrite) : m_file(file), m_overwrite(overwrite)
{
    m_fileAppender.open(m_file, (m_overwrite ? FileWriteMode::OVERWRITE : FileWriteMode::APPEND));
}

void LogBasicFileSink::write(std::string message)
{
    m_fileAppender.write_line(message);
    m_fileAppender.flush();
}

void LogBasicFileSink::flush()
{
    m_fileAppender.flush();
}

}  // namespace logging
