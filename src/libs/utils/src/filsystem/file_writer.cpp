#include "utils/file_writer.h"

#include <fstream>
#include <stdexcept>
#include <string_view>

#include "common/common_error_code.h"
#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_logger.h"
#include "internal/utils/filesystem_utils_internal.h"
#include "utils/date_time_utils.h"
#include "utils/filesystem_utils.h"
#include "utils/thread_utils.h"
#include "utils/utils_error_code.h"

#define MODE_STR(mode) ((mode) ? "overwrite" : "append")

namespace utils::filesystem {
using namespace utils::filesystem::internal;

struct FileWriter::Impl {
    std::string file;
    std::string directory;
    std::ofstream stream;
    std::ios::openmode mode{(std::ios::out | std::ios::trunc)};
    ErrorCode errcode{ERR_COMM_SUCCESS};
    size_t currSize{0};

    explicit Impl(std::string_view file)
        : file(to_absolute_path(file)), directory(get_directory(file))
    {
    }
};

FileWriter::~FileWriter()
{
    if (_impl == nullptr) {
        return;
    }
    close();
    _impl.reset();
}

FileWriter::FileWriter(std::string_view file) : _impl(std::make_unique<Impl>(file))
{
    if (file.empty()) {
        _impl->errcode = ERR_COMM_PARAM_EMPTY;
        DEBUG_LOGGER_ERR("Create FileWriter failed. File path is empty.");
        throw std::invalid_argument("Create FileWriter failed. File path is empty.");
        return;
    }

    _impl->file = to_absolute_path(file);
    _impl->directory = get_directory(_impl->file);
}

ErrorCode FileWriter::open_it(bool overwrite)
{
    close();
    if (overwrite) {
        _impl->mode = (std::ios::out | std::ios::trunc);
    } else {
        _impl->mode = (std::ios::out | std::ios::app);
    }

    _impl->stream = std::ofstream(_impl->file, _impl->mode);

    date_time::sleep_ms(constants::filesystem::FILE_OPEN_INTERNAL_MS);

    if (!_impl->stream.is_open()) {
        std::error_code ec(errno, std::generic_category());
        set_thread_last_err(ConvertSysEcToErrorCode(ec));
        DEBUG_LOGGER_ERR("Open file failed. file: \"{}\", mode: {}, msg: {}",
                         _impl->file.data(),
                         MODE_STR(overwrite),
                         get_thread_last_err_msg());
        _impl->errcode = get_thread_last_err();
        return _impl->errcode;
    }

    _impl->currSize = get_file_size(_impl->file);

    DEBUG_LOGGER_DBG(
        "Open file success. file: \"{}\", mode: {}.", _impl->file.data(), MODE_STR(overwrite));
    _impl->errcode = ERR_COMM_SUCCESS;
    set_thread_last_err(_impl->errcode);
    return _impl->errcode;
}

size_t FileWriter::get_file_size_it() const
{
#if OS_WINDOWS
    return get_file_size(_impl->file);
#else
    // tellp() returns a signed position type (std::streampos); handle possible -1 and avoid
    // implicit conversion to unsigned size_t which would change signedness.
    auto pos = _impl->stream.tellp();
    if (pos == std::ofstream::pos_type(-1)) {
        return 0;
    } else {
        return static_cast<size_t>(pos);
    }
#endif
}

ErrorCode FileWriter::open(bool overwrite)
{
    close();
    if (!file_exists(_impl->file) && get_thread_last_err() == ERR_UTILS_NOT_FILE) {
        DEBUG_LOGGER_ERR("Write to text file failed. file: \"{}\". message: \"{}\".",
                         _impl->file,
                         get_thread_last_err_msg());
        _impl->errcode = get_thread_last_err();
        return _impl->errcode;
    }

    if (!create_dir(_impl->directory)) {
        _impl->errcode = get_thread_last_err();
        return _impl->errcode;
    }

    return open_it(overwrite);
}

ErrorCode FileWriter::reopen(bool overwrite)
{
    return open_it(overwrite);
}

void FileWriter::close()
{
    _impl->errcode = ERR_COMM_SUCCESS;
    set_thread_last_err(_impl->errcode);
    if (_impl->stream.is_open()) {
        _impl->stream.flush();
        _impl->stream.close();
        _impl->currSize = 0;
        DEBUG_LOGGER_DBG("Close file success. file: \"{}\".", _impl->file.data());
    }
}

void FileWriter::write(std::string_view str)
{
    if (!_impl->stream.is_open()) {
        _impl->errcode = ERR_UTILS_FILE_NOT_OPEN;
        DEBUG_LOGGER_ERR("Write failed. file: \"{}\", msg: {}.",
                         _impl->file.c_str(),
                         get_utils_err_msg(_impl->errcode));
    } else {
        _impl->errcode = ERR_COMM_SUCCESS;
        _impl->stream << str;
        _impl->currSize += str.length();
    }
    set_thread_last_err(_impl->errcode);
}

void FileWriter::write_line(std::string_view str)
{
    if (!_impl->stream.is_open()) {
        _impl->errcode = ERR_UTILS_FILE_NOT_OPEN;
        DEBUG_LOGGER_ERR("Write line failed. file: \"{}\", msg: {}.",
                         _impl->file.c_str(),
                         get_utils_err_msg(_impl->errcode));
    } else {
        _impl->errcode = ERR_COMM_SUCCESS;
        _impl->stream << str << '\n';
        _impl->currSize += str.length() + constants::filesystem::LF_LENGTH;
    }
    set_thread_last_err(_impl->errcode);
}

void FileWriter::flush()
{
    _impl->errcode = ERR_COMM_SUCCESS;
    _impl->stream.flush();
    _impl->currSize = get_file_size_it();
}

size_t FileWriter::size() const
{
    return _impl->stream.is_open() ? _impl->currSize : get_file_size(_impl->file);
}

std::string FileWriter::file_name_stem() const
{
    return get_file_name_stem(_impl->file);
}

std::string FileWriter::file_name() const
{
    return get_file_name(_impl->file);
}

std::string FileWriter::directory() const
{
    return _impl->directory;
}

std::string FileWriter::full_path() const
{
    return _impl->file;
}

std::string FileWriter::extension() const
{
    return get_extension(_impl->file);
}

ErrorCode FileWriter::get_last_error() const
{
    return _impl->errcode;
}

}  // namespace utils::filesystem
