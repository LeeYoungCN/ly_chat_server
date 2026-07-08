#include "logging/sinks/rotating_file_sink.h"

#include <algorithm>
#include <cstdint>
#include <deque>
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
#include "utils/date_time_utils.h"
#include "utils/file_writer.h"
#include "utils/filesystem_utils.h"
#include "utils/thread_utils.h"

namespace {
constexpr uint32_t DELETE_FILE_RETRY = 3;
constexpr uint32_t DELETE_FILE_SLEEP_MS = 100;

constexpr uint32_t RENAME_FILE_RETRY = 3;
constexpr uint32_t RENAME_FILE_SLEEP_MS = 100;
}  // namespace

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

struct RotatingFileSink::Impl {
    FileWriter _logWriter;
    uint32_t _maxFileSize{0};
    uint32_t _maxFiles{0};
    const std::string _filePath;
    std::deque<LogFileInfo> _logQueue;

    Impl(std::string_view file, uint32_t maxFileSize, uint32_t maxFiles)
        : _logWriter(file),
          _maxFileSize(maxFileSize),
          _maxFiles(maxFiles),
          _filePath(_logWriter.full_path())
    {
    }
};

RotatingFileSink::~RotatingFileSink()
{
    if (_pimpl == nullptr) {
        return;
    }

    _pimpl->_logWriter.flush();
    _pimpl->_logWriter.close();

    _pimpl.reset();
}

RotatingFileSink::RotatingFileSink() : RotatingFileSink(internal::get_default_log_file("log")) {}

RotatingFileSink::RotatingFileSink(std::string_view file, bool rotateOnOpen)
    : RotatingFileSink(file, RotatingFileSink::DEFAULT_MAX_FILE_SIZE,
                       RotatingFileSink::DEFAULT_MAX_FILES, rotateOnOpen)
{
}

RotatingFileSink::RotatingFileSink(std::string_view file, uint32_t maxFileSize, uint32_t maxFiles,
                                   bool rotateOnOpen)
{
    if (maxFileSize == 0) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. maxFileSize is 0.");
        set_thread_last_err(ERR_COMM_PARAM_INVALID);
        throw std::invalid_argument("maxFileSize is 0.");
    }

    if (maxFiles > RotatingFileSink::MAX_FILES) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. maxFiles out of range.");
        set_thread_last_err(ERR_COMM_PARAM_INVALID);
        throw std::out_of_range("maxFiles out of range.");
    }

    _pimpl = std::make_unique<Impl>(file, maxFileSize, maxFiles);

    if (_pimpl->_logWriter.open(false) != ERR_COMM_SUCCESS) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. File: \"{}\", mode: {}. msg: \"{}\".",
                         file,
                         get_file_mode_str(rotateOnOpen),
                         get_comm_err_msg(_pimpl->_logWriter.get_last_error()));
        _pimpl.reset();
        throw std::runtime_error("Failed to open file: " + std::string(file));
    }

    std::string parameter =
        std::format("RotatingFileSink, File: \"{}\", MaxFileSize: {}, MaxFiles: {}",
                    file,
                    maxFileSize,
                    maxFiles);
    set_parameter(parameter);

    init_file_list();

    if (rotateOnOpen) {
        rotate();
    }
}

std::vector<std::string> RotatingFileSink::get_rotating_file_list()
{
    std::lock_guard lock(sink_mutex());
    std::vector<std::string> rst;
    rst.reserve(_pimpl->_logQueue.size());
    for (auto& i : _pimpl->_logQueue) {
        rst.push_back(i.file);
    }
    return rst;
}

void RotatingFileSink::set_max_file_size(uint32_t maxFileSize)
{
    std::lock_guard lock(sink_mutex());
    if (maxFileSize > 0) {
        _pimpl->_maxFileSize = maxFileSize;
    } else {
        DEBUG_LOGGER_ERR("maxFileSize invalid: {}.", maxFileSize);
    }
}

uint32_t RotatingFileSink::max_file_size()
{
    std::lock_guard lock(sink_mutex());
    return _pimpl->_maxFileSize;
}

void RotatingFileSink::set_max_files(uint32_t maxFiles)
{
    std::lock_guard lock(sink_mutex());
    if (maxFiles <= RotatingFileSink::MAX_FILES) {
        _pimpl->_maxFiles = maxFiles;
    } else {
        DEBUG_LOGGER_ERR("maxFiles invalid: {}. maxFiles should be less than or equal to {}.",
                         maxFiles,
                         RotatingFileSink::MAX_FILES);
    }
}

uint32_t RotatingFileSink::max_files()
{
    std::lock_guard lock(sink_mutex());
    return _pimpl->_maxFiles;
}

void RotatingFileSink::flush_it()
{
    if (_pimpl == nullptr) {
        return;
    }
    _pimpl->_logWriter.flush();
}

void RotatingFileSink::sink_it(std::string_view message)
{
    if (_pimpl == nullptr) {
        return;
    }
    if (message.size() + _pimpl->_logWriter.size() > _pimpl->_maxFileSize) {
        rotate();
    }
    _pimpl->_logWriter.write_line(message);
}

void RotatingFileSink::init_file_list()
{
    auto logDir = get_directory(_pimpl->_filePath);

    std::vector<LogFileInfo> logList;
    for (const auto& entry : std::filesystem::directory_iterator(logDir)) {
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

    for (const auto& logInfo : logList) {
        DEBUG_LOGGER_DBG("Find rotating log file. {}", logInfo.to_string());
        _pimpl->_logQueue.emplace_back(logInfo);
    }
}

uint32_t RotatingFileSink::parse_log_index(std::string_view fileName)
{
    constexpr uint32_t MIN_SUFFIX_LEN = 2;  // .1
    constexpr uint32_t MAX_SUFFIX_LEN = 6;  // .20000

    const std::string& logFileName = _pimpl->_logWriter.file_name();

    if (fileName.size() < logFileName.size() + MIN_SUFFIX_LEN ||
        fileName.size() > logFileName.size() + MAX_SUFFIX_LEN) {
        return 0;
    }

    uint32_t i = 0;
    for (; i < logFileName.length(); ++i) {
        if (fileName.at(i) != logFileName.at(i)) {
            return 0;
        }
    }

    if (fileName.at(i++) != '.') {
        return 0;
    }

    if (fileName.at(i) == '0') {
        return 0;
    }

    uint32_t idx = 0;
    for (; i < fileName.size(); ++i) {
        auto c = fileName.at(i);
        if (c >= '0' && c <= '9') {
            idx = idx * 10 + static_cast<uint32_t>(c - '0');
        } else {
            return 0;
        }
    }

    return idx;
}

void RotatingFileSink::rotate()
{
    _pimpl->_logWriter.close();

    if (_pimpl->_maxFiles == 0) {
        _pimpl->_logWriter.reopen(true);
        delete_overflow_file();
        return;
    }

    uint32_t nextIdx = (_pimpl->_logQueue.empty() ? 0 : _pimpl->_logQueue.back().idx) + 1;

    if (nextIdx > RotatingFileSink::MAX_INDEX) {
        nextIdx = RotatingFileSink::MIN_INDEX;
        DEBUG_LOGGER_DBG("Rotate log file wrap around. nextIdx: {}.", nextIdx);
    }

    std::string nextFile = _pimpl->_filePath + "." + std::to_string(nextIdx);

    if (internal::rename_file(
            _pimpl->_filePath, nextFile, true, RENAME_FILE_RETRY, RENAME_FILE_SLEEP_MS)) {
        _pimpl->_logQueue.emplace_back(nextIdx, nextFile);
        _pimpl->_logWriter.reopen(true);
        DEBUG_LOGGER_DBG("Rotate log file success. {}", _pimpl->_logQueue.back().to_string());
    } else {
        _pimpl->_logWriter.reopen(false);
    }

    delete_overflow_file();
}

void RotatingFileSink::delete_overflow_file()
{
    if (_pimpl->_logQueue.empty() || _pimpl->_logQueue.size() <= _pimpl->_maxFiles) {
        return;
    }

    std::deque<LogFileInfo> failedQueue;

    // 保证剩余文件不超过最大文件数量，直到把能删除的都删了。
    while (_pimpl->_logQueue.size() + failedQueue.size() > _pimpl->_maxFiles &&
           !_pimpl->_logQueue.empty()) {
        auto fileInfo = _pimpl->_logQueue.front();
        if (!internal::delete_file(fileInfo.file, DELETE_FILE_RETRY, DELETE_FILE_SLEEP_MS)) {
            failedQueue.emplace_back(fileInfo);
        }
        _pimpl->_logQueue.pop_front();
        DEBUG_LOGGER_DBG("Delete rotating log file. {}", fileInfo.to_string());
    }

    while (!failedQueue.empty()) {
        _pimpl->_logQueue.emplace_front(failedQueue.back());
        failedQueue.pop_back();
    }
}

}  // namespace logging
