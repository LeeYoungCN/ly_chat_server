#include "logging/sinks/daily_file_sink.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "common/common_error_code.h"
#include "common/constants/date_time_constants.h"
#include "common/debug/debug_logger.h"
#include "common/types/date_time_types.h"
#include "internal/logging_internal.h"
#include "logging/sinks/base_rotating_file_sink.h"
#include "utils/date_time_utils.h"
#include "utils/file_writer.h"
#include "utils/filesystem_utils.h"
#include "utils/thread_utils.h"

namespace logging {
using namespace utils::filesystem;
using namespace utils::date_time;
using namespace logging::internal;
using namespace constants::date_time;

constexpr char SPLIT_CHAR = '.';

DailyFileSink::DailyFileSink()
    : DailyFileSink(get_default_log_file(), DEFAULT_ROTATION_HOUR, DEFAULT_ROTATION_MINUTE,
                    DEFAULT_MAX_FILES)
{
}

DailyFileSink::DailyFileSink(std::string_view baseFile)
    : DailyFileSink(baseFile, DEFAULT_ROTATION_HOUR, DEFAULT_ROTATION_MINUTE, DEFAULT_MAX_FILES)
{
}

DailyFileSink::DailyFileSink(std::string_view baseFile, uint32_t hour, uint32_t minute,
                             uint32_t maxFiles)
    : BaseRotatingFileSink(
          baseFile, maxFiles, RotatingPolicy::RENAME, "daliy log file",
          std::format("DailyFileSink. file: \"{}\", hour: {}, minute: {}, maxFiles: {}.", baseFile,
                      hour, minute, maxFiles)),
      _hour(hour),
      _minute(minute)
{
    if (base_file_it().empty()) {
        set_thread_last_err(ERR_COMM_PARAM_EMPTY);
        DEBUG_LOGGER_ERR("Create DailyFileSink failed. baseFile empty.");
        throw std::invalid_argument("baseFile empty.");
    }

    if (_hour < MIN_HOUR || _hour > MAX_HOUR) {
        set_thread_last_err(ERR_COMM_PARAM_OUT_OF_RANGE);
        DEBUG_LOGGER_ERR("Create DailyFileSink failed. hour invalid: {}.", _hour);
        throw std::out_of_range("hour out of range.");
    }

    if (_minute < MIN_MINUTE || _minute > MAX_MINUTE) {
        set_thread_last_err(ERR_COMM_PARAM_OUT_OF_RANGE);
        DEBUG_LOGGER_ERR("Create DailyFileSink failed. minute invalid: {}.", hour);
        throw std::out_of_range("minute out of range.");
    }

    if (maxFiles > MAX_FILES || maxFiles < MIN_FILES) {
        DEBUG_LOGGER_ERR("Create DailyFileSink failed. maxFiles out of range: {}.", maxFiles);
        set_thread_last_err(ERR_COMM_PARAM_OUT_OF_RANGE);
        throw std::out_of_range("maxFiles out of range.");
    }

    TimestampMs now = get_now_timestamp_ms();
    DateTimeSt dateTime = timestamp_to_date_time(now);
    dateTime.hour = _hour;
    dateTime.minute = _minute;
    dateTime.second = 0;
    dateTime.millis = 0;
    _rotateTime = date_time_to_timestamp(dateTime);

    if (_rotateTime < now) {
        _rotateTime += MILLIS_PER_DAY;
    }
    _fileTime = _rotateTime - MILLIS_PER_DAY;

    _fileWriter = std::make_shared<FileWriter>(base_file_it());
    _fileWriter->open(false);
    if (_fileWriter->get_last_error() != ERR_COMM_SUCCESS) {
        DEBUG_LOGGER_ERR("Create DailyFileSink failed. Open file failed. file: {}.",
                         _fileWriter->full_path());
        throw std::runtime_error("Open log file failed.");
    }

    init_file_queue();
}

void DailyFileSink::log_it(const details::LogMsg& logMsg)
{
    if (logMsg.timestamp > _rotateTime) {
        if (_fileWriter->size() > 0) {
            std::string newFile = calc_log_file(_fileTime);
            rotate(newFile);
        }
        while (_rotateTime < logMsg.timestamp) {
            _rotateTime += MILLIS_PER_DAY;
        }
        _fileTime = _rotateTime - MILLIS_PER_DAY;
    }

    std::string content;
    formatter()->format(logMsg, content);
    sink_it(content);
}

TimestampMs DailyFileSink::parse_log_timestamp(std::string_view filename)
{
    if (filename.size() != get_filename(base_file_it()).size() + 9) {
        return 0;
    }

    if (get_extension(filename) != extention_it()) {
        return 0;
    }

    uint32_t idx = 0;
    auto fileStem = filename_stem_it();
    for (; idx < fileStem.size(); ++idx) {
        if (filename[idx] != fileStem[idx]) {
            return 0;
        }
    }

    if (filename[idx++] != SPLIT_CHAR) {
        return 0;
    }

    DateTimeSt dateTime;
    auto parse_number = [&](uint32_t len, uint32_t& number) -> bool {
        for (uint32_t i = 0; i < len; ++i) {
            char c = filename[idx++];
            if (c < '0' || c > '9') {
                return false;
            }
            number = number * 10 + static_cast<uint32_t>(c - '0');
        }
        return true;
    };

    TimestampMs timestamp = 0;
    if (parse_number(4, dateTime.year) && parse_number(2, dateTime.month) &&
        parse_number(2, dateTime.day)) {
        timestamp = date_time_to_timestamp(dateTime);
    }

    return timestamp;
}

std::string DailyFileSink::calc_log_file(TimestampMs time)
{
    auto dateTime = timestamp_to_date_time(time);
    auto filename = std::format("{}{}{:04}{:02}{:02}{}",
                                filename_stem_it(),
                                SPLIT_CHAR,
                                dateTime.year,
                                dateTime.month,
                                dateTime.day,
                                extention_it());
    return join_paths({directory_it(), filename});
}

void DailyFileSink::init_file_queue()
{
    std::vector<std::pair<TimestampMs, std::string>> logList;
    for (const auto& entry : std::filesystem::directory_iterator(directory_it())) {
        if (!entry.is_regular_file()) {
            continue;
        }

        TimestampMs timestamp = parse_log_timestamp(entry.path().filename().string());
        if (timestamp > _fileTime) {
            continue;
        }

        std::string file = entry.path().string();
        if (calc_log_file(timestamp) == file) {
            logList.emplace_back(timestamp, file);
        }
    }

    std::sort(logList.begin(), logList.end());
    for (const auto& fileInfo : logList) {
        push_back_file(fileInfo.second);
        DEBUG_LOGGER_DBG("Find daily log file. file: \"{}\"", fileInfo.second);
    }
}

}  // namespace logging
