#include "logging/sinks/basic_file_sink.h"

#include "utils/file_writer.h"
#include "utils/filesystem_utils.h"
#include "utils/process_utils.h"

namespace logging {
using namespace utils::filesystem;
using namespace utils::process;

struct BasicFileSink::Impl {
    Impl(std::string_view file, bool overwrite) : _filePath(file), _overwrite(overwrite)
    {
        _fileWriter.open(_filePath,
                         (_overwrite ? FileWriteMode::OVERWRITE : FileWriteMode::APPEND));
    }

    std::string _filePath;
    bool _overwrite{true};
    utils::filesystem::FileWriter _fileWriter;
};

BasicFileSink::BasicFileSink()
{
    std::string process = get_proc_path();

    _pimpl = new Impl(join_paths({get_directory(process), "logs", get_file_name(process) + ".log"}),
                      true);

    set_parameter("BasicFileSink, File: " + _pimpl->_filePath +
                  ", Overwrite: " + std::to_string(_pimpl->_overwrite));
}

BasicFileSink::~BasicFileSink()
{
    if (_pimpl == nullptr) {
        return;
    }

    _pimpl->_fileWriter.flush();
    _pimpl->_fileWriter.close();

    delete _pimpl;
    _pimpl = nullptr;
}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite)
    : _pimpl(new Impl(to_absolute_path(file), overwrite))
{
    set_parameter("BasicFileSink, File: " + std::string(file) +
                  ", Overwrite: " + std::to_string(overwrite));
}

void BasicFileSink::sink_it(std::string_view message)
{
    _pimpl->_fileWriter.write_line(message);
}

void BasicFileSink::flush_it()
{
    _pimpl->_fileWriter.flush();
}

}  // namespace logging
