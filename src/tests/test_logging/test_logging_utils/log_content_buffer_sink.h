#ifndef TEST_LOGGING_TEST_UTILS_LOG_CONTENT_BUFFER_SINK_H
#define TEST_LOGGING_TEST_UTILS_LOG_CONTENT_BUFFER_SINK_H

#include <sys/types.h>

#include <cstdint>
#include <vector>

#include "logging/sinks/base_sink.h"

using namespace logging;

namespace test::test_logging {
class LogContentBufferSink : public BaseSink {
public:
    LogContentBufferSink();
    ~LogContentBufferSink() override = default;
    explicit LogContentBufferSink(uint32_t capacity);

    [[nodiscard]] uint32_t capacity() const;
    const std::vector<std::string>& buffer();
    const std::vector<std::string>& disk();
    void clear();

protected:
    void sink_it(std::string_view message) override;
    void flush_it() override;

private:
    uint32_t _capacity;
    std::vector<std::string> _buffer;
    std::vector<std::string> _disk;
};
}  // namespace test::test_logging

#endif  // TEST_LOGGING_TEST_UTILS_LOG_CONTENT_BUFFER_SINK_H
