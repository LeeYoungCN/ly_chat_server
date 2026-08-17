#ifndef UTILS_FILE_WRITER_H
#define UTILS_FILE_WRITER_H

#include <memory>
#include <string>
#include <string_view>

#include "common/types/error_code_types.h"
#include "common/types/filesystem_types.h"

namespace origin::utils::filesystem {
class FileWriterImpl;
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

    [[nodiscard]] FileSize size() const;
    [[nodiscard]] std::string filename_stem() const;
    [[nodiscard]] std::string filename() const;
    [[nodiscard]] std::string directory() const;
    [[nodiscard]] std::string full_path() const;
    [[nodiscard]] std::string extension() const;
    [[nodiscard]] ErrorCode get_last_error() const;

private:
    std::unique_ptr<FileWriterImpl> _pimpl;
};

}  // namespace origin::utils::filesystem

#endif  // UTILS_FILE_WRITER_H
