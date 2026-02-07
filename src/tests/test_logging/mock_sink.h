#include <cstdint>
#include <string>
#include <vector>

#include "logging/sinks/sink.h"

namespace test::test_logging {
class MockSink : public logging::Sink {
public:
    MockSink()
    {
        _buffer.reserve(_capacity);
        _disk.reserve(_capacity);
    }

    explicit MockSink(uint32_t capacity) : _capacity(capacity)
    {
        _buffer.reserve(_capacity);
        _disk.reserve(_capacity);
    }

    ~MockSink() override = default;

    const std::vector<std::string>& buffer() const { return _buffer; }
    const std::vector<std::string>& disk() const { return _disk; }
    [[nodiscard]] uint32_t capacity() const { return _capacity; }

protected:
    void sink_it(std::string_view message) override
    {
        _buffer.emplace_back(message);
        if (_buffer.size() >= _capacity) {
            flush_it();
        }
    }

    void flush_it() override
    {
        _disk.reserve(_disk.size() + _buffer.size());
        _disk.insert(_disk.end(), _buffer.begin(), _buffer.end());
        _buffer.clear();
    }

private:
    uint32_t _capacity = 1024;
    std::vector<std::string> _buffer;
    std::vector<std::string> _disk;
};
}  // namespace test::test_logging
