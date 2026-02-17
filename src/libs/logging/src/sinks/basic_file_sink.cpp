#include "logging/sinks/basic_file_sink.h"

#include "common/utils/file_writer.h"
#include "common/utils/filesystem_utils.h"
#include "common/utils/process_utils.h"

namespace logging {
using namespace common::filesystem;
using namespace common::process;

BasicFileSink::BasicFileSink()
{
    std::string process = get_proc_path();
    _filePath = JoinPaths({GetDirectory(process), "logs", GetFileName(process) + ".log"});
    _fileWriter.open(_filePath, FileWriteMode::OVERWRITE);
}

BasicFileSink::~BasicFileSink()
{
    _fileWriter.flush();
    _fileWriter.close();
}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite)
    : _filePath(ToAbsolutePath(file)), _overwrite(overwrite)
{
    _fileWriter.open(_filePath, (_overwrite ? FileWriteMode::OVERWRITE : FileWriteMode::APPEND));
}

void BasicFileSink::sink_it(std::string_view message)
{
    _fileWriter.write_line(message);
}

void BasicFileSink::flush_it()
{
    _fileWriter.flush();
}

}  // namespace logging
