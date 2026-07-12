#include "logging/sinks/basic_rotating_file_sink.h"

#include <cstdint>
#include <deque>
#include <format>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include "common/debug/debug_logger.h"
#include "internal/logging_internal.h"

namespace logging {
constexpr uint32_t DELETE_FILE_RETRY = 3;
constexpr uint32_t DELETE_FILE_SLEEP_MS = 10;

BasicRotatingFileSink::BasicRotatingFileSink(std::string_view itemName, uint32_t maxFiles)
    : _itemName(itemName), _maxFiles(maxFiles)
{
}

std::vector<std::string> BasicRotatingFileSink::get_file_list()
{
    std::lock_guard lock(_mtx);
    std::vector<std::string> rst;
    rst.reserve(_fileQue.size());
    for (auto& i : _fileQue) {
        rst.push_back(i);
    }
    return rst;
}

void BasicRotatingFileSink::push_back_file(std::string_view file)
{
    std::lock_guard lock(_mtx);
    _fileQue.emplace_back(file);
    DEBUG_LOGGER_TRACE("Enqueue {}. {}", _itemName, file);
}

const std::string& BasicRotatingFileSink::back()
{
    std::lock_guard lock(_mtx);
    return _fileQue.back();
}

const std::string& BasicRotatingFileSink::front()
{
    std::lock_guard lock(_mtx);
    return _fileQue.front();
}

void BasicRotatingFileSink::delete_overflow_file()
{
    std::lock_guard lock(_mtx);

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

}  // namespace logging
