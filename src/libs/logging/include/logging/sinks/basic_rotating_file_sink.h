#ifndef LOGGING_SINKS_BASIC_ROTATING_FILE_SINK_H
#define LOGGING_SINKS_BASIC_ROTATING_FILE_SINK_H

#include <cstdint>
#include <deque>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

namespace logging {
class BasicRotatingFileSink {
public:
    BasicRotatingFileSink(std::string_view itemName, uint32_t maxFiles);
    ~BasicRotatingFileSink() = default;

public:
    std::vector<std::string> get_file_list();

protected:
    void push_back_file(std::string_view file);

    const std::string& back();
    const std::string& front();

protected:
    virtual void init_file_queue() = 0;
    virtual void rotate(std::string_view newFile) = 0;
    virtual void delete_overflow_file();

private:
    std::string _itemName;
    uint32_t _maxFiles{0};
    std::deque<std::string> _fileQue;
    std::mutex _mtx;
};

}  // namespace logging

#endif  // LOGGING_SINKS_BASIC_ROTATING_FILE_SINK_H
