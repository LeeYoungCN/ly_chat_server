#ifndef UTILS_IMPL_FILE_WRITER_IMPL_H
#define UTILS_IMPL_FILE_WRITER_IMPL_H
#include <cstddef>
#include <fstream>
#include <string>
#include <string_view>

#include "common/common_error_code.h"
#include "common/types/error_code_types.h"

namespace origin::utils::filesystem {

class FileWriterImpl {
public:
    FileWriterImpl() = delete;
    explicit FileWriterImpl(std::string_view file);
    ~FileWriterImpl() = default;

    ErrorCode open(bool overwrite = true);
    ErrorCode reopen(bool overwrite = true);
    void close();
    void write(std::string_view str);
    void write_line(std::string_view str);
    void flush();

    [[nodiscard]] size_t size();
    [[nodiscard]] std::string filename_stem() const;
    [[nodiscard]] std::string filename() const;
    [[nodiscard]] std::string directory() const;
    [[nodiscard]] std::string full_path() const;
    [[nodiscard]] std::string extension() const;
    [[nodiscard]] ErrorCode get_last_error() const;

private:
    ErrorCode open_it(bool overwrite);
    [[nodiscard]] size_t get_file_size_it();

private:
    std::string _file;
    std::ofstream _stream;
    std::ios::openmode _mode{(std::ios::out | std::ios::trunc)};
    ErrorCode _errcode{ERR_COMM_SUCCESS};
    size_t _currSize{0};
};
}  // namespace origin::utils::filesystem
#endif  // UTILS_IMPL_FILE_WRITER_IMPL_H
