#include "logging/sinks/basic_file_sink.h"

#include <stdexcept>

#include "common/debug/debug_logger.h"
#include "internal/logging_internal.h"
#include "utils/file_writer.h"
#include "utils/filesystem_utils.h"
#include "utils/utils_error_code.h"

namespace logging {
using namespace utils::filesystem;

struct BasicFileSink::Impl {
    Impl(std::string_view file, bool overwrite) : _filePath(to_absolute_path(file)), _overwrite(overwrite)
    {
        _fileWriter.open(_filePath,
                         (_overwrite ? FileWriteMode::OVERWRITE : FileWriteMode::APPEND));
    }

    std::string _filePath;
    bool _overwrite{true};
    utils::filesystem::FileWriter _fileWriter;
};

BasicFileSink::~BasicFileSink()
{
    if (_pimpl == nullptr) {
        return;
    }

    _pimpl->_fileWriter.flush();
    _pimpl->_fileWriter.close();

    _pimpl.reset();
    _pimpl = nullptr;
}

BasicFileSink::BasicFileSink() : BasicFileSink(internal::get_default_log_file("log"), true) {}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite)
{
    if (file.empty()) {
        DEBUG_LOGGER_ERR("Failed to create BasicFileSink. file path is empty.");
        throw std::invalid_argument("File path cannot be empty");
    }

    _pimpl = std::make_unique<Impl>(file, overwrite);

    if (_pimpl->_fileWriter.get_last_error() != ERR_COMM_SUCCESS) {
        DEBUG_LOGGER_ERR("Failed to open file. file: \"{}\", overwrite: {}, msg: {}",
                         file,
                         overwrite,
                         get_utils_err_msg(_pimpl->_fileWriter.get_last_error()));
        _pimpl.reset();
        throw std::runtime_error("Failed to open file: " + std::string(file));
    }

    set_parameter("BasicFileSink, File: \"" + std::string(file) +
                  "\", Overwrite: " + std::to_string(overwrite));
}

void BasicFileSink::sink_it(std::string_view message)
{
    if (_pimpl == nullptr) {
        return;
    }
    _pimpl->_fileWriter.write_line(message);
}

void BasicFileSink::flush_it()
{
    if (_pimpl == nullptr) {
        return;
    }
    _pimpl->_fileWriter.flush();
}

}  // namespace logging
