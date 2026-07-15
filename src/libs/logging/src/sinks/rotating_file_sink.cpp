#include "logging/sinks/rotating_file_sink.h"

#include <algorithm>
#include <cstdint>
#include <filesystem>
#include <format>
#include <mutex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include "common/common_error_code.h"
#include "common/debug/debug_logger.h"
#include "internal/logging_internal.h"
#include "logging/details/log_msg.h"
#include "logging/sinks/base_rotating_file_sink.h"
#include "utils/date_time_utils.h"
#include "utils/file_writer.h"
#include "utils/filesystem_utils.h"
#include "utils/thread_utils.h"

namespace logging {
using namespace utils::filesystem;

struct LogFileInfo {
    uint32_t idx{0};
    std::string file;
    TimestampMs modifyTime{0};
    size_t fileSize{0};
    LogFileInfo(uint32_t i, std::string_view f)
        : idx(i), file(f), modifyTime(get_file_modify_time(f)), fileSize(get_file_size(f))
    {
    }

    [[nodiscard]] std::string to_string() const
    {
        return std::format(
            "idx: {}, time: [{}], size: {}, file: \"{}\"",
            idx,
            utils::date_time::format_time_string(modifyTime, "%Y-%m-%d %H:%M:%S.%3f"),
            fileSize,
            file);
    }
};

RotatingFileSink::RotatingFileSink() : RotatingFileSink(internal::get_default_log_file("log")) {}

RotatingFileSink::RotatingFileSink(std::string_view file, bool rotateOnOpen)
    : RotatingFileSink(file, RotatingFileSink::DEFAULT_MAX_FILE_SIZE,
                       RotatingFileSink::DEFAULT_MAX_FILES, rotateOnOpen)
{
}

RotatingFileSink::RotatingFileSink(std::string_view file, uint32_t maxFileSize, uint32_t maxFiles,
                                   bool rotateOnOpen)
    : BaseRotatingFileSink(
          file, false, maxFiles, "rotating log file",
          std::format("RotatingFileSink, File: \"{}\", MaxFileSize: {}, MaxFiles: {}.", file,
                      maxFileSize, maxFiles)),
      _maxFileSize(maxFileSize)
{
    if (maxFileSize == 0) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. maxFileSize is 0.");
        set_thread_last_err(ERR_COMM_PARAM_OUT_OF_RANGE);
        throw std::invalid_argument("maxFileSize is 0.");
    }

    if (maxFiles > RotatingFileSink::MAX_FILES) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. maxFiles out of range.");
        set_thread_last_err(ERR_COMM_PARAM_INVALID);
        throw std::out_of_range("maxFiles out of range.");
    }

    init_file_queue();

    if (rotateOnOpen) {
        rotate(get_next_file());
    }
}

void RotatingFileSink::set_max_file_size(uint32_t maxFileSize)
{
    std::lock_guard lock(_sinkMtx);
    if (maxFileSize > 0) {
        _maxFileSize = maxFileSize;
    } else {
        DEBUG_LOGGER_ERR("maxFileSize invalid: {}.", maxFileSize);
    }
}

uint32_t RotatingFileSink::max_file_size()
{
    std::lock_guard lock(_sinkMtx);
    return _maxFileSize;
}

void RotatingFileSink::set_max_files(uint32_t maxFiles)
{
    std::lock_guard lock(_sinkMtx);
    if (maxFiles <= RotatingFileSink::MAX_FILES) {
        set_max_files_it(maxFiles);
    } else {
        DEBUG_LOGGER_ERR("maxFiles invalid: {}. maxFiles should be less than or equal to {}.",
                         maxFiles,
                         RotatingFileSink::MAX_FILES);
    }
}

uint32_t RotatingFileSink::max_files()
{
    return max_files_it();
}

void RotatingFileSink::log_it(const details::LogMsg& logMsg)
{
    std::string content;
    _formatter->format(logMsg, content);

    if (content.size() + _fileWriter.size() > _maxFileSize) {
        rotate(get_next_file());
    }
    sink_it(content);
}

void RotatingFileSink::init_file_queue()
{
    std::vector<LogFileInfo> logList;
    for (const auto& entry : std::filesystem::directory_iterator(directory())) {
        if (!entry.is_regular_file()) {
            continue;
        }

        uint32_t idx = parse_log_index(entry.path().filename().string());

        if (idx < RotatingFileSink::MIN_INDEX || idx > RotatingFileSink::MAX_INDEX) {
            continue;
        }

        logList.emplace_back(idx, entry.path().string());
    }

    std::sort(
        logList.begin(), logList.end(), [](const LogFileInfo& a, const LogFileInfo& b) -> bool {
            if (a.modifyTime == b.modifyTime) {
                return a.idx < b.idx;
            } else {
                return a.modifyTime < b.modifyTime;
            };
        });

    set_next_idx(logList.empty() ? MIN_INDEX : logList.back().idx + 1);
    for (const auto& logInfo : logList) {
        DEBUG_LOGGER_DBG("Find rotating log file. {}", logInfo.to_string());
        push_back_file(logInfo.file);
    }
}

std::string RotatingFileSink::get_next_file()
{
    return _file + "." + std::to_string(get_next_idx());
}

void RotatingFileSink::set_next_idx(uint32_t idx)
{
    if (idx > RotatingFileSink::MAX_INDEX) {
        idx = RotatingFileSink::MIN_INDEX;
        DEBUG_LOGGER_DBG("Rotate log file wrap around. nextIdx: {}.", _nextIdx);
    }
    _nextIdx = idx;
}

uint32_t RotatingFileSink::get_next_idx()
{
    uint32_t idx = _nextIdx++;

    if (_nextIdx > RotatingFileSink::MAX_INDEX) {
        _nextIdx = RotatingFileSink::MIN_INDEX;
        DEBUG_LOGGER_DBG("Rotate log file wrap around. nextIdx: {}.", _nextIdx);
    }
    return idx;
}

uint32_t RotatingFileSink::parse_log_index(std::string_view filename)
{
    constexpr uint32_t MIN_SUFFIX_LEN = 2;  // .1
    constexpr uint32_t MAX_SUFFIX_LEN = 6;  // .20000

    if (filename.size() < _filename.size() + MIN_SUFFIX_LEN ||
        filename.size() > _filename.size() + MAX_SUFFIX_LEN) {
        return 0;
    }

    uint32_t i = 0;
    for (; i < _filename.length(); ++i) {
        if (filename.at(i) != _filename.at(i)) {
            return 0;
        }
    }

    if (filename.at(i++) != '.') {
        return 0;
    }

    if (filename.at(i) == '0') {
        return 0;
    }

    uint32_t idx = 0;
    for (; i < filename.size(); ++i) {
        auto c = filename.at(i);
        if (c >= '0' && c <= '9') {
            idx = idx * 10 + static_cast<uint32_t>(c - '0');
        } else {
            return 0;
        }
    }

    return idx;
}

}  // namespace logging
