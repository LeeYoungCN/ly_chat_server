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
#include "logging/sinks/basic_file_sink.h"

namespace logging {
using namespace utils::filesystem;
constexpr uint32_t DELETE_FILE_RETRY = 3;
constexpr uint32_t DELETE_FILE_SLEEP_MS = 10;
constexpr uint32_t RENAME_FILE_RETRY = 3;
constexpr uint32_t RENAME_FILE_SLEEP_MS = 10;

BaseRotatingFileSink::BaseRotatingFileSink(std::string_view file, bool overwrite, uint32_t maxFiles,
                                           std::string_view itemName, std::string_view paramStr)
    : BasicFileSink(file, overwrite, paramStr), _maxFiles(maxFiles), _itemName(itemName)
{
}

std::vector<std::string> BaseRotatingFileSink::get_file_list()
{
    std::lock_guard lock(_sinkMtx);
    std::vector<std::string> rst;
    rst.reserve(_fileQue.size());
    for (auto& i : _fileQue) {
        rst.push_back(i);
    }
    return rst;
}
void BaseRotatingFileSink::set_max_files_it(uint32_t maxFiles)
{
    _maxFiles.store(maxFiles);
}

uint32_t BaseRotatingFileSink::max_files_it()
{
    return _maxFiles.load();
}

void BaseRotatingFileSink::push_back_file(std::string_view file)
{
    _fileQue.emplace_back(file);
    DEBUG_LOGGER_TRACE("Enqueue {}. {}", _itemName, file);
}

void BaseRotatingFileSink::rotate(std::string_view newFile)
{
    _fileWriter.close();

    if (internal::rename_file(file(), newFile, true, RENAME_FILE_RETRY, RENAME_FILE_SLEEP_MS)) {
        _fileWriter.reopen(true);
        push_back_file(newFile);
        DEBUG_LOGGER_DBG("Rotate log file success. {}", newFile);
    } else {
        _fileWriter.reopen(false);
    }

    delete_overflow_file();
}

void BaseRotatingFileSink::delete_overflow_file()
{
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

}  // namespace logging
