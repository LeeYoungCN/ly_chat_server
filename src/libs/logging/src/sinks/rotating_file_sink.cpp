#include "logging/sinks/rotating_file_sink.h"

#include <algorithm>
#include <cstdint>
#include <deque>
#include <filesystem>
#include <format>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include "common/debug/debug_logger.h"
#include "internal/logging_internal.h"
#include "utils/file_writer.h"
#include "utils/filesystem_utils.h"
#include "utils/thread_utils.h"

namespace logging {
using namespace utils::filesystem;

struct LogFileInfo {
    uint32_t idx{0};
    std::string file;

    bool operator<(const LogFileInfo& other) const { return idx < other.idx; }
};

struct RotatingFileSink::Impl {
    const std::string _filePath;
    uint32_t _maxFileSize{0};
    uint32_t _maxFiles{0};
    const bool _overwrite{true};
    FileWriter _logWriter;
    std::deque<LogFileInfo> _logQueue;

    Impl(std::string_view file, uint32_t maxFileSize, uint32_t maxFiles, bool overwrite)
        : _filePath(to_absolute_path(file)),
          _maxFileSize(maxFileSize),
          _maxFiles(maxFiles),
          _overwrite(overwrite),
          _logWriter(_filePath)
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

RotatingFileSink::RotatingFileSink(std::string_view file, uint32_t maxFileSize, uint32_t maxFiles,
                                   bool overwrite)
{
    if (file.empty()) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. file path is empty.");
        set_thread_last_err(ERR_COMM_PARAM_NULL);
        throw std::invalid_argument("file empty.");
    }

    if (maxFileSize == 0) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. maxFileSize is 0.");
        set_thread_last_err(ERR_COMM_PARAM_INVALID);
        throw std::invalid_argument("maxFileSize is 0.");
    }

    if (maxFiles == 0) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. maxFiles is 0.");
        set_thread_last_err(ERR_COMM_PARAM_INVALID);
        throw std::invalid_argument("maxFiles is 0.");
    }

    _pimpl = std::make_unique<Impl>(file, maxFileSize, maxFiles, overwrite);

    if (_pimpl->_logWriter.open(overwrite) != ERR_COMM_SUCCESS) {
        DEBUG_LOGGER_ERR("Create RotatingFileSink failed. File: \"{}\", mode: {}. msg: \"{}\".",
                         file,
                         (overwrite ? "overwrite" : "append"),
                         get_comm_err_msg(_pimpl->_logWriter.get_last_error()));
        _pimpl.reset();
        throw std::runtime_error("Failed to open file: " + std::string(file));
    }

    std::string parameter =
        std::format("RotatingFileSink, File: \"{}\", MaxFileSize: {}, MaxFiles: {}, Mode: {}",
                    file,
                    maxFileSize,
                    maxFiles,
                    (overwrite ? "overwrite" : "append"));

    set_parameter(parameter);

    init_file_list();
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
    sink_mutex();
    if (maxFileSize > 0) {
        _pimpl->_maxFileSize = maxFileSize;
    } else {
        DEBUG_LOGGER_ERR("maxFileSize invalid, {}.", maxFileSize);
    }
}

void RotatingFileSink::set_max_files(uint32_t maxFiles)
{
    sink_mutex();
    if (maxFiles > 0) {
        _pimpl->_maxFiles = maxFiles;
    } else {
        DEBUG_LOGGER_ERR("maxFiles invalid, {}.", maxFiles);
    }
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

        if (idx < ROTATING_FILE_SINK_MIN_INDEX) {
            continue;
        }

        logList.emplace_back(idx, entry.path().string());
    }

    std::sort(logList.begin(),
              logList.end(),
              [](const LogFileInfo& a, const LogFileInfo& b) -> bool { return a.idx < b.idx; });

    for (const auto& logInfo : logList) {
        DEBUG_LOGGER_DBG(
            "Find rotating log file. idx: {}, file: \"{}\".", logInfo.idx, logInfo.file);
        _pimpl->_logQueue.emplace_back(logInfo);
    }
}

uint32_t RotatingFileSink::parse_log_index(std::string_view fileName)
{
    constexpr uint32_t MIN_SUFFIX_LEN = 2;  // .1

    const std::string& logFileName = _pimpl->_logWriter.file_name();

    if (fileName.size() < logFileName.size() + MIN_SUFFIX_LEN) {
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

    uint32_t nextIdx = (_pimpl->_logQueue.empty() ? 0 : _pimpl->_logQueue.back().idx) + 1;
    std::string nextFile = _pimpl->_filePath + "." + std::to_string(nextIdx);

    if (rename_log_file(nextFile)) {
        _pimpl->_logQueue.emplace_back(nextIdx, nextFile);
        _pimpl->_logWriter.reopen(true);
    } else {
        _pimpl->_logWriter.reopen(false);
    }

    delete_overflow_file();
}

bool RotatingFileSink::rename_log_file(std::string_view nextFile)
{
    constexpr uint32_t MAX_RENAME_CNT = 3;
    for (uint32_t i = 0; i < MAX_RENAME_CNT; i++) {
        if (rename_file(_pimpl->_filePath, nextFile, false)) {
            DEBUG_LOGGER_DBG("Rotating file success. file: \"{}\", size: {}.",
                             nextFile,
                             get_file_size(nextFile));
            return true;
        }
    }
    DEBUG_LOGGER_DBG("Rotating file failed. file: \"{}\", size: {}, msg: \"{}\".",
                     nextFile,
                     get_file_size(_pimpl->_filePath),
                     get_thread_last_err_msg());
    return false;
}

void RotatingFileSink::delete_overflow_file()
{
    std::deque<LogFileInfo> failedQueue;

    // 保证剩余文件不超过最大文件数量，直到把能删除的都删了。
    while (_pimpl->_logQueue.size() + failedQueue.size() > _pimpl->_maxFiles &&
           !_pimpl->_logQueue.empty()) {
        auto fileInfo = _pimpl->_logQueue.front();
        if (!delete_file(fileInfo.file)) {
            failedQueue.emplace_back(fileInfo);
        }
        _pimpl->_logQueue.pop_front();
    }

    while (!failedQueue.empty()) {
        _pimpl->_logQueue.emplace_front(failedQueue.back());
        failedQueue.pop_back();
    }
}

bool RotatingFileSink::delete_log_file(std::string_view file)
{
    constexpr uint32_t MAX_FAILED_CNT = 3;

    for (uint32_t i = 0; i < MAX_FAILED_CNT; i++) {
        if (delete_file(file)) {
            DEBUG_LOGGER_DBG("Delete rotating log file success. file: \"{}\".", file);
            return true;
        }
    }

    DEBUG_LOGGER_ERR("Delete rotating log file faild. file: \"{}\", msg: \"{}\".",
                     file,
                     get_thread_last_err_msg());
    return false;
}

}  // namespace logging
