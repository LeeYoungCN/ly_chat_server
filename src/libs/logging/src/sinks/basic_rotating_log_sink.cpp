#include "logging/sinks/basic_rotating_log_sink.h"

#include <cstdint>
#include <deque>
#include <format>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include "common/debug/debug_logger.h"
#include "internal/logging_internal.h"
#include "utils/date_time_utils.h"
#include "utils/filesystem_utils.h"

using namespace utils::filesystem;
namespace logging {
constexpr uint32_t DELETE_FILE_RETRY = 3;
constexpr uint32_t DELETE_FILE_SLEEP_MS = 10;

LogFileInfo::LogFileInfo(uint32_t i, std::string_view f)
    : idx(i), file(f), modifyTime(get_file_modify_time(f)), fileSize(get_file_size(f))
{
}

[[nodiscard]] std::string LogFileInfo::to_string() const
{
    return std::format("idx: {}, time: [{}], size: {}, file: \"{}\".",
                       idx,
                       utils::date_time::format_time_string(modifyTime, "%Y-%m-%d %H:%M:%S.%3f"),
                       fileSize,
                       file);
}

BasicRotatingLogSink::BasicRotatingLogSink(uint32_t maxFiles, uint32_t minIdx, uint32_t maxIdx,
                                           bool unlimited)
    : _maxFiles(maxFiles), _minIdx(minIdx), _maxIdx(maxIdx), _unlimited(unlimited)
{
}

std::vector<std::string> BasicRotatingLogSink::get_file_list()
{
    std::lock_guard lock(_mtx);
    std::vector<std::string> rst;
    rst.reserve(_fileQue.size());
    for (auto& i : _fileQue) {
        rst.push_back(i.file);
    }
    return rst;
}

uint32_t BasicRotatingLogSink::nextIdx()
{
    std::lock_guard lock(_mtx);
    uint32_t nextIdx = (_fileQue.empty() ? _minIdx : _fileQue.back().idx + 1);

    if (nextIdx > _maxIdx) {
        nextIdx = _minIdx;
    }
    return nextIdx;
}

void BasicRotatingLogSink::enqueue(std::string_view file)
{
    enqueue(nextIdx(), file);
}

void BasicRotatingLogSink::enqueue(uint32_t idx, std::string_view file)
{
    std::lock_guard lock(_mtx);
    _fileQue.emplace_back(idx, file);
    DEBUG_LOGGER_DBG("Enqueue roating log file. {}", _fileQue.back().to_string());
}

void BasicRotatingLogSink::dequeue()
{
    std::lock_guard lock(_mtx);
    _fileQue.pop_front();
}

const LogFileInfo& BasicRotatingLogSink::back()
{
    std::lock_guard lock(_mtx);
    return _fileQue.back();
}

const LogFileInfo& BasicRotatingLogSink::front()
{
    std::lock_guard lock(_mtx);
    return _fileQue.front();
}

void BasicRotatingLogSink::delete_overflow_file()
{
    std::lock_guard lock(_mtx);
    if (_unlimited) {
        return;
    }
    if (_fileQue.empty() || _fileQue.size() <= _maxFiles) {
        return;
    }

    std::deque<LogFileInfo> failedQueue;

    // 保证剩余文件不超过最大文件数量，直到把能删除的都删了。
    while (_fileQue.size() + failedQueue.size() > _maxFiles && !_fileQue.empty()) {
        auto fileInfo = _fileQue.front();
        if (!internal::delete_file(fileInfo.file, DELETE_FILE_RETRY, DELETE_FILE_SLEEP_MS)) {
            failedQueue.emplace_back(fileInfo);
        }
        _fileQue.pop_front();
        DEBUG_LOGGER_DBG("Delete rotating log file. {}", fileInfo.to_string());
    }

    while (!failedQueue.empty()) {
        _fileQue.emplace_front(failedQueue.back());
        failedQueue.pop_back();
    }
}

}  // namespace logging
