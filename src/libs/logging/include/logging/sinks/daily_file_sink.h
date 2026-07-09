#pragma once

#ifndef LOGGING_SINKS_DAILY_FILE_SINK_H
#define LOGGING_SINKS_DAILY_FILE_SINK_H
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#include "common/types/date_time_types.h"
#include "logging/sinks/basic_rotating_log_sink.h"
#include "logging/sinks/sink.h"
#include "utils/file_writer.h"

namespace logging {

class DailyFileSink : public Sink, public BasicRotatingLogSink {
public:
    static constexpr uint32_t DEFAULT_ROTATION_HOUR = 0;
    static constexpr uint32_t DEFAULT_ROTATION_MINUTE = 0;
    static constexpr uint32_t DEFAULT_MAX_FILES = 0;
    static constexpr uint32_t UNLIMIT_MAX_FILES = 0;

public:
    DailyFileSink();
    explicit DailyFileSink(std::string_view baseFile, bool overwrite = true);
    DailyFileSink(std::string_view baseFile, uint32_t hour, uint32_t minute,
                  uint32_t maxFiles = UNLIMIT_MAX_FILES, bool overwrite = true);

    ~DailyFileSink() override = default;

public:
    void log(const details::LogMsg& logMsg) override;
    void flush() override;

public:
    std::string log_file();

protected:
    void init_file_queue() override;
    void rotate() override;

private:
    TimestampMs parse_log_timestamp(std::string_view filename);
    std::string calc_log_file(TimestampMs timestamp);

private:
    std::string _baseFile;
    uint32_t _rotationHour{DEFAULT_ROTATION_HOUR};
    uint32_t _rotationMinute{DEFAULT_ROTATION_MINUTE};
    bool _overwrite{true};

    std::string _directory;
    std::string _fileStem;
    std::string _extention;

    TimestampMs _rotateTime{0};
    std::shared_ptr<utils::filesystem::FileWriter> _fileWriter;
};

}  // namespace logging

#endif  // LOGGING_SINKS_DAILY_FILE_SINK_H
