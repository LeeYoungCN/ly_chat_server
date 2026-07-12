#include "logging/sinks/base_rotating_file_sink.h"

#include <cstdint>
#include <deque>
#include <format>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include "common/debug/debug_logger.h"
#include "internal/logging_internal.h"
#include "utils/filesystem_utils.h"

namespace logging {
using namespace utils::filesystem;
constexpr uint32_t DELETE_FILE_RETRY = 3;
constexpr uint32_t DELETE_FILE_SLEEP_MS = 10;
constexpr uint32_t RENAME_FILE_RETRY = 3;
constexpr uint32_t RENAME_FILE_SLEEP_MS = 10;

BaseRotatingFileSink::BaseRotatingFileSink(std::string_view logFile, uint32_t maxFiles,
                                           RotatingPolicy rotatingPolicy, std::string_view itemName,
                                           std::string_view paramStr)
    : Sink(paramStr),
      _baseFile(to_absolute_path(logFile)),
      _directory(get_directory(_baseFile)),
      _fileStem(get_filename_stem(_baseFile)),
      _extention(get_extension(_baseFile)),
      _maxFiles(maxFiles),
      _itemName(itemName),
      _policy(rotatingPolicy)
{
}

BaseRotatingFileSink::~BaseRotatingFileSink()
{
    if (_fileWriter == nullptr) {
        return;
    }

    _fileWriter->flush();
    _fileWriter->close();
}

std::string BaseRotatingFileSink::log_file()
{
    std::lock_guard lock(sink_mutex());
    return _fileWriter->full_path();
}

std::vector<std::string> BaseRotatingFileSink::get_file_list()
{
    std::lock_guard lock(sink_mutex());
    std::vector<std::string> rst;
    rst.reserve(_fileQue.size());
    for (auto& i : _fileQue) {
        rst.push_back(i);
    }
    return rst;
}

void BaseRotatingFileSink::log(const details::LogMsg& logMsg)
{
    std::lock_guard lock(sink_mutex());
    log_it(logMsg);
}

void BaseRotatingFileSink::flush()
{
    std::lock_guard lock(sink_mutex());
    flush_it();
}

void BaseRotatingFileSink::sink_it(std::string_view message)
{
    _fileWriter->write_line(message);
}

void BaseRotatingFileSink::flush_it()
{
    _fileWriter->flush();
}

void BaseRotatingFileSink::push_back_file(std::string_view file)
{
    _fileQue.emplace_back(file);
    DEBUG_LOGGER_TRACE("Enqueue {}. {}", _itemName, file);
}

void BaseRotatingFileSink::rotate(std::string_view newFile)
{
    if (_policy == RotatingPolicy::OPEN_NEW) {
        rotate_open_new(newFile);
    } else {
        rotate_replace(newFile);
    }
}

void BaseRotatingFileSink::rotate_open_new(std::string_view newFile)
{
    _fileWriter->close();
    _fileWriter = std::make_shared<utils::filesystem::FileWriter>(newFile);
    _fileWriter->open(true);
    push_back_file(newFile);
    delete_overflow_file();
}

void BaseRotatingFileSink::rotate_replace(std::string_view newFile)
{
    std::string src = _fileWriter->full_path();
    _fileWriter->close();

    if (internal::rename_file(src, newFile, true, RENAME_FILE_RETRY, RENAME_FILE_SLEEP_MS)) {
        _fileWriter->reopen(true);
        push_back_file(newFile);
        DEBUG_LOGGER_DBG("Rotate log file success. {}", newFile);
    } else {
        _fileWriter->reopen(false);
    }

    delete_overflow_file();
}

void BaseRotatingFileSink::delete_overflow_file()
{
    if (_fileQue.empty() || _fileQue.size() <= _maxFiles) {
        return;
    }

    // 保证剩余文件不超过最大文件数量，直到把能删除的都删了。
    while (_fileQue.size() > _maxFiles) {
        auto file = _fileQue.front();
        if (!internal::delete_file(file, DELETE_FILE_RETRY, DELETE_FILE_SLEEP_MS)) {
            DEBUG_LOGGER_ERR("Delete {} failed. {}", _itemName, file);
        } else {
            DEBUG_LOGGER_DBG("Delete {} sucess. {}", _itemName, file);
        }
        _fileQue.pop_front();
    }
}

const std::string& BaseRotatingFileSink::base_file_it() const
{
    return _baseFile;
}

const std::string& BaseRotatingFileSink::directory_it() const
{
    return _directory;
}
const std::string& BaseRotatingFileSink::filename_stem_it() const
{
    return _fileStem;
}

const std::string& BaseRotatingFileSink::extention_it() const
{
    return _extention;
}

}  // namespace logging
