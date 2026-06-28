#include "utils/file_writer.h"

#include <fstream>
#include <stdexcept>
#include <string_view>

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

FileWriter::~FileWriter()
{
    close();
}

FileWriter::FileWriter(std::string_view file)
{
    if (file.empty()) {
        m_errcode = ERR_COMM_PARAM_EMPTY;
        DEBUG_LOGGER_ERR("Create FileWriter failed. File path is empty.");
        throw std::invalid_argument("Create FileWriter failed. File path is empty.");
        return;
    }

    m_file = to_absolute_path(file);
    m_directory = get_directory(m_file);
}

ErrorCode FileWriter::open_it(bool overwrite)
{
    close();
    if (overwrite) {
        m_mode = (std::ios::out | std::ios::trunc);
    } else {
        m_mode = (std::ios::out | std::ios::app);
    }

    m_stream = std::ofstream(m_file, m_mode);

    date_time::sleep_ms(constants::filesystem::FILE_OPEN_INTERNAL_MS);

    if (!m_stream.is_open()) {
        std::error_code ec(errno, std::generic_category());
        set_thread_last_err(ConvertSysEcToErrorCode(ec));
        DEBUG_LOGGER_ERR("Open file failed. file: \"{}\", mode: {}, msg: {}",
                         m_file.data(),
                         MODE_STR(overwrite),
                         get_thread_last_err_msg());
        m_errcode = get_thread_last_err();
        return m_errcode;
    }

    m_currSize = get_file_size(m_file);

    DEBUG_LOGGER_DBG(
        "Open file success. file: \"{}\", mode: {}.", m_file.data(), MODE_STR(overwrite));
    m_errcode = ERR_COMM_SUCCESS;
    set_thread_last_err(m_errcode);
    return m_errcode;
}

ErrorCode FileWriter::open(bool overwrite)
{
    close();
    if (!file_exists(m_file) && get_thread_last_err() == ERR_UTILS_NOT_FILE) {
        DEBUG_LOGGER_ERR("Write to text file failed. file: \"{}\". message: \"{}\".",
                         m_file,
                         get_thread_last_err_msg());
        m_errcode = get_thread_last_err();
        return m_errcode;
    }

    if (!create_dir(m_directory)) {
        m_errcode = get_thread_last_err();
        return m_errcode;
    }

    return open_it(overwrite);
}

ErrorCode FileWriter::reopen(bool overwrite)
{
    return open_it(overwrite);
}

void FileWriter::close()
{
    m_errcode = ERR_COMM_SUCCESS;
    set_thread_last_err(m_errcode);
    if (m_stream.is_open()) {
        m_stream.flush();
        m_stream.close();
        m_currSize = 0;
        DEBUG_LOGGER_DBG("Close file success. file: \"{}\".", m_file.data());
    }
}

void FileWriter::write(std::string_view str)
{
    if (!m_stream.is_open()) {
        m_errcode = ERR_UTILS_FILE_NOT_OPEN;
        DEBUG_LOGGER_ERR(
            "Write failed. file: \"{}\", msg: {}.", m_file.c_str(), get_utils_err_msg(m_errcode));
    } else {
        m_errcode = ERR_COMM_SUCCESS;
        m_stream << str;
        m_currSize += str.length();
    }
    set_thread_last_err(m_errcode);
}

void FileWriter::write_line(std::string_view str)
{
    if (!m_stream.is_open()) {
        m_errcode = ERR_UTILS_FILE_NOT_OPEN;
        DEBUG_LOGGER_ERR("Write line failed. file: \"{}\", msg: {}.",
                         m_file.c_str(),
                         get_utils_err_msg(m_errcode));
    } else {
        m_errcode = ERR_COMM_SUCCESS;
        m_stream << str << '\n';
        m_currSize += str.length() + constants::filesystem::LF_LENGTH;
    }
    set_thread_last_err(m_errcode);
}

void FileWriter::flush()
{
    m_errcode = ERR_COMM_SUCCESS;
    m_stream.flush();
    m_currSize = get_file_size(m_file);
}

size_t FileWriter::size() const
{
    return m_stream.is_open() ? m_currSize : get_file_size(m_file);
}

std::string FileWriter::file_name_stem() const
{
    return get_file_name_stem(m_file);
}

std::string FileWriter::file_name() const
{
    return get_file_name(m_file);
}

std::string FileWriter::directory() const
{
    return m_directory;
}

std::string FileWriter::full_path() const
{
    return m_file;
}

std::string FileWriter::extension() const
{
    return get_extension(m_file);
}

ErrorCode FileWriter::get_last_error() const
{
    return m_errcode;
}

}  // namespace utils::filesystem
