#ifndef UTILS_FILE_WRITER_H
#define UTILS_FILE_WRITER_H

#include <cstddef>
#include <cstdio>
#include <fstream>
#include <string>
#include <string_view>

#include "common/common_error_code.h"
#include "common/types/error_code_types.h"

namespace utils::filesystem {

class FileWriter {
public:
    FileWriter() = delete;
    explicit FileWriter(std::string_view file);
    ~FileWriter();

    ErrorCode open(bool overwrite = true);
    ErrorCode reopen(bool overwrite = true);
    void close();
    void write(std::string_view str);
    void write_line(std::string_view str);
    void flush();

    [[nodiscard]] size_t size() const;
    [[nodiscard]] std::string file_name_stem() const;
    [[nodiscard]] std::string file_name() const;
    [[nodiscard]] std::string directory() const;
    [[nodiscard]] std::string full_path() const;
    [[nodiscard]] std::string extension() const;
    [[nodiscard]] ErrorCode get_last_error() const;

private:
    ErrorCode open_it(bool overwrite);
    std::string m_file;
    std::string m_directory;
    std::ofstream m_stream;
    std::ios::openmode m_mode{(std::ios::out | std::ios::trunc)};
    ErrorCode m_errcode{ERR_COMM_SUCCESS};
    size_t m_currSize{0};
};

}  // namespace utils::filesystem

#endif  // UTILS_FILE_WRITER_H
