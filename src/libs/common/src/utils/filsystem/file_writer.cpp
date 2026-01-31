#include "common/utils/file_writer.h"

#include <fstream>
#include <string_view>

#include "common/common_error_code.h"
#include "common/constants/filesystem_constants.h"
#include "common/debug/debug_logger.h"
#include "common/utils/date_time_utils.h"
#include "common/utils/error_code_utils.h"
#include "common/utils/filesystem_utils.h"
#include "internal/common/utils/filesystem_utils_internal.h"

namespace {
const char* FileWriteModeStr(common::utils::filesystem::FileWriteMode mode)
{
    if (mode == common::utils::filesystem::OVERWRITE) {
        return "overwrite";
    } else {
        return "append";
    }
}
}  // namespace

namespace common::utils::filesystem {
using namespace common::utils::filesystem::internal;

FileWriter::~FileWriter()
{
    close();
}

ErrorCode FileWriter::open_(FileWriteMode mode)
{
    close();
    if (mode == FileWriteMode::OVERWRITE) {
        m_mode = (std::ios::out | std::ios::trunc);
    } else {
        m_mode = (std::ios::out | std::ios::app);
    }

    m_stream = std::ofstream(m_file, m_mode);
    date_time::SleepMS(constants::filesystem::FILE_OPEN_INTERNAL_MS);
    if (!m_stream.is_open()) {
        m_errcode = get_thread_last_err();
        std::error_code ec(errno, std::generic_category());
        ConvertSysEcToErrorCode(ec);
        DEBUG_LOGGER_ERR("Open file failed. file: {}, mode: {}, msg: {}",
                      m_file.data(),
                      FileWriteModeStr(mode),
                      get_thread_last_err_msg());
        return m_errcode;
    }
    m_currSize = GetFileSize(m_file);
    DEBUG_LOGGER_DBG("Open file success. file: {}, mode: {}", m_file.data(), FileWriteModeStr(mode));
    m_errcode = ERR_COMM_SUCCESS;
    return m_errcode;
}

ErrorCode FileWriter::open(std::string_view file, FileWriteMode mode)
{
    close();
    if (file.empty()) {
        m_errcode = ERR_COMM_PARAM_EMPTY;
        DEBUG_LOGGER_ERR("Open file failed. msg: {}.", get_comm_err_msg(m_errcode));
        return m_errcode;
    }
    m_file = ToAbsolutePath(file);
    m_directory = GetDirectory(m_file);

    if (!CreateDir(m_directory)) {
        m_errcode = get_thread_last_err();
        return m_errcode;
    }

    return open_(mode);
}

ErrorCode FileWriter::reopen(FileWriteMode mode)
{
    return open_(mode);
}

void FileWriter::close()
{
    m_errcode = ERR_COMM_SUCCESS;
    if (m_stream.is_open()) {
        m_stream.close();
    }
}

void FileWriter::write(std::string_view str)
{
    if (!m_stream.is_open()) {
        set_thread_last_err(ERR_COMM_FILE_NOT_OPEN);
        m_errcode = ERR_COMM_FILE_NOT_OPEN;
        DEBUG_LOGGER_ERR("Append failed. file: {}, msg: {}.", m_file.c_str(), get_comm_err_msg(m_errcode));
    } else {
        m_errcode = ERR_COMM_SUCCESS;
        m_stream << str;
        m_currSize += str.length();
    }
}

void FileWriter::write_line(std::string_view str)
{
    if (!m_stream.is_open()) {
        m_errcode = ERR_COMM_FILE_NOT_OPEN;
        DEBUG_LOGGER_ERR("Append failed. file: {}, msg: {}.", m_file.c_str(), get_comm_err_msg(m_errcode));
    } else {
        m_errcode = ERR_COMM_SUCCESS;
        m_stream << str << '\n';
        m_currSize += str.length() + common::constants::filesystem::LF_LENGTH;
    }
}

void FileWriter::flush()
{
    m_errcode = ERR_COMM_SUCCESS;
    m_stream.flush();
}

size_t FileWriter::size() const
{
    return m_currSize;
}

std::string FileWriter::file_name() const
{
    return GetFileName(m_file);
}
std::string FileWriter::base_name() const
{
    return GetBaseName(m_file);
}

const std::string& FileWriter::directory() const
{
    return m_directory;
}

const std::string& FileWriter::full_path() const
{
    return m_file;
}

ErrorCode FileWriter::get_last_error() const
{
    return m_errcode;
}

}  // namespace common::utils::filesystem
