#include "logging/sinks/basic_file_sink.h"

#include <format>
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

BasicFileSink::~BasicFileSink()
{
    _fileWriter.flush();
    _fileWriter.close();
}

BasicFileSink::BasicFileSink() : BasicFileSink(internal::get_default_log_file("log"), true) {}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite)
    : BasicFileSink(
          file, overwrite,
          std::format("BasicFileSink. file: \"{}\", mode: {}.", file, get_file_mode_str(overwrite)))
{
}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite, std::string_view paramStr)
    : Sink(paramStr),
      _file(to_absolute_path(file)),
      _directory(get_directory(_file)),
      _filename(get_filename(_file)),
      _filenameStem(get_filename_stem(_file)),
      _extention(get_extension(_file)),
      _overwrite(overwrite),
      _fileWriter(_file)
{
    if (_file.empty()) {
        throw std::invalid_argument("file empty");
    }

    _fileWriter.open(_overwrite);
    if (_fileWriter.get_last_error() != ERR_COMM_SUCCESS) {
        DEBUG_LOGGER_ERR("Create BasicFileSink failed. File: \"{}\", mode: {}. msg: \"{}\".",
                         file,
                         get_file_mode_str(_overwrite),
                         get_utils_err_msg(_fileWriter.get_last_error()));

        throw std::runtime_error("Failed to open file: " + std::string(file));
    }
}

const std::string& BasicFileSink::file() const
{
    return _file;
}

const std::string& BasicFileSink::directory() const
{
    return _directory;
}

const std::string& BasicFileSink::filename() const
{
    return _filename;
}

const std::string& BasicFileSink::filename_stem() const
{
    return _filenameStem;
}

const std::string& BasicFileSink::extention() const
{
    return _extention;
}

void BasicFileSink::log_it(const LogMsg& logMsg)
{
    std::string content;
    _formatter->format(logMsg, content);
    sink_it(content);
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
