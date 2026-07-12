#pragma once

#ifndef LOGGING_SINKS_DAILY_FILE_SINK_H
#define LOGGING_SINKS_DAILY_FILE_SINK_H
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "common/types/date_time_types.h"
#include "logging/sinks/basic_rotating_file_sink.h"
#include "logging/sinks/sink.h"
#include "utils/file_writer.h"

namespace logging {
/**
 * @brief: 按照日期滚动日志。
 */
class DailyFileSink : public Sink, public BasicRotatingFileSink {
public:
    static constexpr uint32_t DEFAULT_ROTATION_HOUR = 0;
    static constexpr uint32_t DEFAULT_ROTATION_MINUTE = 0;
    static constexpr uint32_t DEFAULT_MAX_FILES = 30;
    static constexpr uint32_t MIN_FILES = 1;
    static constexpr uint32_t MAX_FILES = 20000;

public:
    DailyFileSink();
    explicit DailyFileSink(std::string_view baseFile);
    DailyFileSink(std::string_view baseFile, uint32_t hour, uint32_t minute,
                  uint32_t maxFiles = DEFAULT_MAX_FILES);

    ~DailyFileSink() override = default;

public:
    void log(const details::LogMsg& logMsg) override;
    void flush() override;

public:
    std::string log_file();

protected:
    void init_file_queue() override;
    void rotate(std::string_view newFile) override;

private:
    TimestampMs parse_log_timestamp(std::string_view filename);
    std::string calc_log_file(TimestampMs time);

private:
    std::string _file;
    uint32_t _hour{DEFAULT_ROTATION_HOUR};
    uint32_t _minute{DEFAULT_ROTATION_MINUTE};
    uint32_t _maxFiles{DEFAULT_MAX_FILES};

    std::string _directory;
    std::string _fileStem;
    std::string _extention;

    TimestampMs _fileTime{0};
    TimestampMs _rotateTime{0};
    std::shared_ptr<utils::filesystem::FileWriter> _fileWriter;
};

}  // namespace logging

#endif  // LOGGING_SINKS_DAILY_FILE_SINK_H
