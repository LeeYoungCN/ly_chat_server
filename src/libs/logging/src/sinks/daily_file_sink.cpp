#include "logging/sinks/daily_file_sink.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include "common/common_error_code.h"
#include "common/constants/date_time_constants.h"
#include "common/debug/debug_logger.h"
#include "common/types/date_time_types.h"
#include "internal/logging_internal.h"
#include "logging/sinks/basic_rotating_file_sink.h"
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
    : Sink(std::format("DailyFileSink, File: \"{}\", hour: {}, minute: {}, maxFiles: {}.", baseFile,
                       hour, minute, maxFiles)),
      BasicRotatingFileSink("daliy log file", maxFiles),
      _file(to_absolute_path(baseFile)),
      _hour(hour),
      _minute(minute),
      _maxFiles(maxFiles),
      _directory(get_directory(_file)),
      _fileStem(get_filename_stem(_file)),
      _extention(get_extension(_file))
{
    if (_file.empty()) {
        set_thread_last_err(ERR_COMM_PARAM_EMPTY);
        DEBUG_LOGGER_ERR("Create DailyFileSink failed. baseFile empty.");
        throw std::invalid_argument("baseFile empty.");
    }

    if (_hour < MIN_HOUR || _hour > MAX_HOUR) {
        set_thread_last_err(ERR_COMM_PARAM_OUT_OF_RANGE);
        DEBUG_LOGGER_ERR("Create DailyFileSink failed. hour invalid: {}.", hour);
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
    _rotateTime = date_time_to_timestamp(dateTime);

    if (_rotateTime < now) {
        _rotateTime += MILLIS_PER_DAY;
    }
    _fileTime = _rotateTime - MILLIS_PER_DAY;

    _fileWriter = std::make_shared<FileWriter>(calc_log_file(_fileTime));
    _fileWriter->open(false);
    if (_fileWriter->get_last_error() != ERR_COMM_SUCCESS) {
        DEBUG_LOGGER_ERR("Create DailyFileSink failed. Open file failed. file: {}.",
                         _fileWriter->full_path());
        throw std::runtime_error("Open log file failed.");
    }

    init_file_queue();
}

void DailyFileSink::log(const details::LogMsg& logMsg)
{
    std::lock_guard lock(sink_mutex());
    if (logMsg.timestamp > _rotateTime) {
        while (_rotateTime < logMsg.timestamp) {
            _rotateTime += MILLIS_PER_DAY;
        }
        _fileTime = _rotateTime - MILLIS_PER_DAY;
        std::string newFile = calc_log_file(_fileTime);
        rotate(newFile);
    }

    std::string content;
    formatter()->format(logMsg, content);
    _fileWriter->write_line(content);
}

void DailyFileSink::flush()
{
    std::lock_guard lock(sink_mutex());
    _fileWriter->flush();
}

std::string DailyFileSink::log_file()
{
    std::lock_guard lock(sink_mutex());
    return _fileWriter->full_path();
}

TimestampMs DailyFileSink::parse_log_timestamp(std::string_view filename)
{
    if (filename.size() != _fileWriter->filename().size()) {
        return 0;
    }

    if (get_extension(filename) != _extention) {
        return 0;
    }

    uint32_t idx = 0;
    for (; idx < _fileStem.size(); ++idx) {
        if (filename[idx] != _fileStem[idx]) {
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
                                _fileStem,
                                SPLIT_CHAR,
                                dateTime.year,
                                dateTime.month,
                                dateTime.day,
                                _extention);
    return join_paths({_directory, filename});
}

void DailyFileSink::init_file_queue()
{
    std::vector<std::pair<TimestampMs, std::string>> logList;
    for (const auto& entry : std::filesystem::directory_iterator(_directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        TimestampMs timestamp = parse_log_timestamp(entry.path().filename().string());
        if (timestamp > _rotateTime - MILLIS_PER_DAY) {
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

void DailyFileSink::rotate(std::string_view newFile)
{
    _fileWriter->close();
    _fileWriter = std::make_shared<FileWriter>(newFile);
    _fileWriter->open(false);
    push_back_file(newFile);
    delete_overflow_file();
}

}  // namespace logging
