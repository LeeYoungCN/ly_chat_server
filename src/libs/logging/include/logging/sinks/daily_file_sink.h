#pragma once

#ifndef LOGGING_SINKS_DAILY_FILE_SINK_H
#define LOGGING_SINKS_DAILY_FILE_SINK_H
#include <cstdint>
#include <string>
#include <string_view>

#include "common/types/date_time_types.h"
#include "logging/sinks/basic_rotating_file_sink.h"

namespace logging {
/**
 * @brief: 按照日期滚动日志。
 */
class DailyFileSink : public BasicRotatingFileSink {
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

protected:
    void init_file_queue() override;

private:
    TimestampMs parse_log_timestamp(std::string_view filename);
    std::string calc_log_file(TimestampMs time);

private:
    uint32_t _hour{DEFAULT_ROTATION_HOUR};
    uint32_t _minute{DEFAULT_ROTATION_MINUTE};

    TimestampMs _fileTime{0};
    TimestampMs _rotateTime{0};
};

}  // namespace logging

#endif  // LOGGING_SINKS_DAILY_FILE_SINK_H
