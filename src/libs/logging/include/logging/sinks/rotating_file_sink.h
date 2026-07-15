#ifndef LOGGING_SINKS_ROTATING_FILE_SINK_H
#define LOGGING_SINKS_ROTATING_FILE_SINK_H

#include <sys/types.h>

#include <cstdint>
#include <string_view>

#include "logging/sinks/base_rotating_file_sink.h"

namespace logging {
class RotatingFileSink : public BaseRotatingFileSink {
public:
    static constexpr uint32_t DEFAULT_MAX_FILE_SIZE = 10 * 1024 * 1024;  // 10MB;
    static constexpr uint32_t DEFAULT_MAX_FILES = 100;                   // 最大保留100个日志文件;

    static constexpr uint32_t MAX_FILES = 20000;  // 最大保留20000个日志文件;

    static constexpr uint32_t MIN_INDEX = 1;
    static constexpr uint32_t MAX_INDEX = MAX_FILES;  // 最大索引号为20000;

public:
    RotatingFileSink();
    ~RotatingFileSink() override = default;
    explicit RotatingFileSink(std::string_view file, bool rotateOnOpen = false);
    RotatingFileSink(std::string_view file, uint32_t maxFileSize, uint32_t maxFiles,
                     bool rotateOnOpen = false);

public:
    void set_max_file_size(uint32_t maxFileSize);
    [[nodiscard]] uint32_t max_file_size();

    void set_max_files(uint32_t maxFiles);
    [[nodiscard]] uint32_t max_files();

protected:
    void log_it(const details::LogMsg& logMsg) override;

private:
    std::string get_next_file();
    uint32_t get_next_idx();
    void set_next_idx(uint32_t idx);
    uint32_t parse_log_index(std::string_view file);
    void init_file_queue() override;

private:
    uint32_t _maxFileSize{0};
    uint32_t _nextIdx{MIN_INDEX};
};
}  // namespace logging
#endif  // LOGGING_SINKS_ROTATING_FILE_SINK_H
