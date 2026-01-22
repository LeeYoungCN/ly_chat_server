#include "logging/sinks/basic_file_sink.h"

#include "common/utils/file_writer.h"
#include "common/utils/filesystem_utils.h"

namespace logging {
using namespace common::utils::filesystem;

BasicFileSink::BasicFileSink()
{
    m_file = JoinPaths({
        GetCurrentWorkingDirectory(),
        "logs",
        GetProcessFileName() + ".log"});
    m_fileWriter.open(m_file, FileWriteMode::OVERWRITE);
}

BasicFileSink::~BasicFileSink()
{
    m_fileWriter.close();
}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite) : m_file(file), m_overwrite(overwrite)
{
    m_fileWriter.open(m_file, (m_overwrite ? FileWriteMode::OVERWRITE : FileWriteMode::APPEND));
}

void BasicFileSink::write(std::string_view message)
{
    m_fileWriter.write_line(message);
    m_fileWriter.flush();
}

void BasicFileSink::flush()
{
    m_fileWriter.flush();
}

}  // namespace logging
