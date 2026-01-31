#ifndef COMMON_UTILS_FILE_WRITER_H
#define COMMON_UTILS_FILE_WRITER_H

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <string>
#include <string_view>

#include "common/common_error_code.h"
#include "common/types/error_code_types.h"

namespace common::filesystem {

enum FileWriteMode {
    OVERWRITE,
    APPEND
};

class FileWriter {
public:
    FileWriter() = default;
    ~FileWriter();

    ErrorCode open(std::string_view file, FileWriteMode mode = FileWriteMode::OVERWRITE);
    ErrorCode reopen(FileWriteMode mode = FileWriteMode::OVERWRITE);
    void close();
    void write(std::string_view str);
    void write_line(std::string_view str);
    void flush();

    [[nodiscard]] size_t size() const;
    [[nodiscard]] std::string file_name() const;
    [[nodiscard]] std::string base_name() const;
    [[nodiscard]] const std::string& directory() const;
    [[nodiscard]] const std::string& full_path() const;
    [[nodiscard]] ErrorCode get_last_error() const;

private:
    ErrorCode open_(FileWriteMode mode);
    std::string m_file;
    std::string m_directory;
    std::ofstream m_stream;
    std::ios::openmode m_mode{(std::ios::out | std::ios::trunc)};
    ErrorCode m_errcode{ERR_COMM_SUCCESS};
    size_t m_currSize{};
};

}  // namespace common::filesystem

#endif  // COMMON_UTILS_FILE_WRITER_H
