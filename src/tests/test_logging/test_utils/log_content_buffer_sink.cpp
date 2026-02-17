#include "log_content_buffer_sink.h"

#include <iostream>
#include <mutex>

namespace test::test_logging {
LogContentBuffer::LogContentBuffer() : LogContentBuffer(1024) {}

LogContentBuffer::LogContentBuffer(uint32_t capacity) : _capacity(capacity)
{
    _buffer.reserve(_capacity);
}

uint32_t LogContentBuffer::capacity() const
{
    return _capacity;
}

const std::vector<std::string>& LogContentBuffer::buffer()
{
    std::lock_guard<std::mutex> lock(sink_mutex());
    return _buffer;
}

const std::vector<std::string>& LogContentBuffer::disk()
{
    std::lock_guard<std::mutex> lock(sink_mutex());
    return _disk;
}

void LogContentBuffer::clear()
{
    _buffer.clear();
    _disk.clear();
}

void LogContentBuffer::sink_it(std::string_view message)
{
    std::cout << message << std::endl;
    _buffer.emplace_back(message);
    if (_buffer.size() >= _capacity) {
        flush_it();
    }
}

void LogContentBuffer::flush_it()
{
    _disk.reserve(_disk.size() + _buffer.size());
    _disk.insert(_disk.end(), _buffer.begin(), _buffer.end());
    _buffer.clear();
}

}  // namespace test::test_logging
