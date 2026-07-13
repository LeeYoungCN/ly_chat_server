#include "logging/sinks/basic_file_sink.h"

#include <format>
#include <mutex>
#include <stdexcept>

#include "common/debug/debug_logger.h"
#include "internal/logging_internal.h"
#include "logging/sinks/sink.h"
#include "utils/file_writer.h"
#include "utils/filesystem_utils.h"
#include "utils/utils_error_code.h"

namespace logging {
using namespace utils::filesystem;
using namespace logging::details;

struct BasicFileSink::Impl {
    explicit Impl(std::string_view file) : _fileWriter(file) {}

    FileWriter _fileWriter;
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
    : Sink(
          std::format("BasicFileSink. file: \"{}\", mode: {}.", file, get_file_mode_str(overwrite)))
{
    _pimpl = std::make_unique<Impl>(to_absolute_path(file));
    _pimpl->_fileWriter.open(overwrite);

    if (_pimpl->_fileWriter.get_last_error() != ERR_COMM_SUCCESS) {
        DEBUG_LOGGER_ERR("Create BasicFileSink failed. File: \"{}\", mode: {}. msg: \"{}\".",
                         file,
                         get_file_mode_str(overwrite),
                         get_utils_err_msg(_pimpl->_fileWriter.get_last_error()));
        _pimpl.reset();
        throw std::runtime_error("Failed to open file: " + std::string(file));
    }

    set_parameter(
        std::format("BasicFileSink, file: \"{}\", mode: {}.", file, get_file_mode_str(overwrite)));
}

std::string BasicFileSink::file()
{
    return _pimpl->_fileWriter.full_path();
}

void BasicFileSink::log_it(const LogMsg& logMsg)
{
    std::string content;
    formatter()->format(logMsg, content);
    sink_it(content);
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
