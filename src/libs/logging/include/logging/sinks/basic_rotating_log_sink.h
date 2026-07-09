#ifndef LOGGING_SINKS_BASIC_ROTATING_LOG_SINK_H
#define LOGGING_SINKS_BASIC_ROTATING_LOG_SINK_H

#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

#include "common/types/date_time_types.h"

namespace logging {
struct RotatingLogSt {
    uint32_t idx{0};
    std::string file;
    TimestampMs modifyTime{0};
    size_t fileSize{0};

    RotatingLogSt(uint32_t i, std::string_view f);

    [[nodiscard]] std::string to_string() const;
};

class BasicRotatingLogSink {
public:
    BasicRotatingLogSink(uint32_t maxFiles, uint32_t minIdx, uint32_t maxIdx, bool unlimited = false);
    ~BasicRotatingLogSink() = default;

public:
    std::vector<std::string> get_file_list();
protected:
    uint32_t nextIdx();

    void enqueue(std::string_view file);
    void enqueue(uint32_t idx, std::string_view file);

    void dequeue();

    const RotatingLogSt& back();
    const RotatingLogSt& front();

protected:
    virtual void init_file_queue() = 0;
    virtual void rotate() = 0;
    virtual void delete_overflow_file();

private:
    uint32_t _maxFiles{0};
    uint32_t _minIdx{0};
    uint32_t _maxIdx{0};
    bool _unlimited{false};
    std::deque<RotatingLogSt> _fileQue;
    std::mutex _mtx;
};

}  // namespace logging

#endif  // LOGGING_SINKS_BASIC_ROTATING_LOG_SINK_H
