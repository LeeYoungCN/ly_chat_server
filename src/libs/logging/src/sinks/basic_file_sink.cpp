#include "logging/sinks/basic_file_sink.h"
#include <mutex>

#include "common/utils/file_writer.h"
#include "common/utils/filesystem_utils.h"

namespace logging {
using namespace common::filesystem;

BasicFileSink::BasicFileSink(): logging::BasicFileSink("", true) { }

BasicFileSink::~BasicFileSink()
{
    _fileWriter.close();
}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite) : _filePath(file), _overwrite(overwrite)
{
    if (_filePath.empty()) {
        _filePath = JoinPaths({
        GetCurrentWorkingDirectory(),
        "logs",
        GetProcessFileName() + ".log"});
    }
    _fileWriter.open(_filePath, (_overwrite ? FileWriteMode::OVERWRITE : FileWriteMode::APPEND));
}

void BasicFileSink::write(std::string_view message)
{
    std::lock_guard lock(_mutex);
    _fileWriter.write_line(message);
}

void BasicFileSink::flush()
{
    std::lock_guard lock(_mutex);
    _fileWriter.flush();
}

}  // namespace logging
