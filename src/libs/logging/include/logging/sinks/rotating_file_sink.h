#ifndef LOGGING_SINKS_ROTATING_FILE_SINK_H
#define LOGGING_SINKS_ROTATING_FILE_SINK_H

#include <sys/types.h>

#include <cstdint>
#include <string_view>
#include <vector>

#include "logging/sinks/base_sink.h"

namespace logging {
class RotatingFileSink : public BaseSink {
public:
    static constexpr uint32_t DEFAULT_MAX_FILE_SIZE = 10 * 1024 * 1024;  // 10MB;
    static constexpr uint32_t DEFAULT_MAX_FILES = 100;                   // 最大保留100个日志文件;

    static constexpr uint32_t MAX_FILES = 20000;  // 最大保留20000个日志文件;

    static constexpr uint32_t MIN_INDEX = 1;
    static constexpr uint32_t MAX_INDEX = MAX_FILES;  // 最大索引号为20000;

public:
    RotatingFileSink();
    ~RotatingFileSink() override;
    explicit RotatingFileSink(std::string_view file, bool rotateOnOpen = false);
    RotatingFileSink(std::string_view file, uint32_t maxFileSize, uint32_t maxFiles,
                     bool rotateOnOpen = false);

public:
    std::string log_file();
    std::vector<std::string> get_file_list();

    void set_max_file_size(uint32_t maxFileSize);
    [[nodiscard]] uint32_t max_file_size();

    void set_max_files(uint32_t maxFiles);
    [[nodiscard]] uint32_t max_files();

protected:
    void flush_it() override;
    void sink_it(std::string_view message) override;

private:
    uint32_t parse_log_index(std::string_view file);
    void init_file_queue();
    void rotate();
    void delete_overflow_file();

private:
    struct Impl;
    std::unique_ptr<Impl> _pimpl;
};
}  // namespace logging
#endif  // LOGGING_SINKS_ROTATING_FILE_SINK_H
