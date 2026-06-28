#ifndef LOGGING_SINKS_ROTATING_FILE_SINK_H
#define LOGGING_SINKS_ROTATING_FILE_SINK_H

#include <sys/types.h>

#include <cstdint>
#include <string_view>
#include <vector>

#include "logging/sinks/base_sink.h"

namespace logging {
constexpr uint32_t ROTATING_FILE_SINK_MIN_INDEX = 1;

constexpr uint32_t ROTATING_FILE_SINK_DEFAULT_MAX_FILE_SIZE = 10 * 1024 * 1024;  // 10MB
constexpr uint32_t ROTATING_FILE_SINK_DEFAULT_MAX_FILES = 1024;

class RotatingFileSink : public BaseSink {
public:
    RotatingFileSink();
    ~RotatingFileSink() override;
    explicit RotatingFileSink(std::string_view file,
                              uint32_t maxFileSize = ROTATING_FILE_SINK_DEFAULT_MAX_FILE_SIZE,
                              uint32_t maxFiles = ROTATING_FILE_SINK_DEFAULT_MAX_FILES,
                              bool overwrite = true);

public:
    std::vector<std::string> get_rotating_file_list();
    void set_max_file_size(uint32_t maxFileSize);
    void set_max_files(uint32_t maxFiles);

protected:
    void flush_it() override;
    void sink_it(std::string_view message) override;

private:
    void init_file_list();
    uint32_t parse_log_index(std::string_view file);

    void rotate();
    bool rename_log_file(std::string_view nextFile);

    void delete_overflow_file();
    bool delete_log_file(std::string_view file);

private:
    struct Impl;
    std::unique_ptr<Impl> _pimpl;
};
}  // namespace logging
#endif  // LOGGING_SINKS_ROTATING_FILE_SINK_H
