#ifndef LOGGING_INTERNAL_SINKS_BASE_ROTATING_FILE_SINK_IMPL_H
#define LOGGING_INTERNAL_SINKS_BASE_ROTATING_FILE_SINK_IMPL_H

#include <atomic>
#include <cstdint>
#include <deque>
#include <string>
#include <string_view>
#include <vector>

#include "internal/sinks/basic_file_sink_impl.h"

namespace logging {

class BaseRotatingFileSinkImpl : public BasicFileSinkImpl {
public:
    BaseRotatingFileSinkImpl() = delete;
    BaseRotatingFileSinkImpl(std::string_view file, bool overwrite, uint32_t maxFiles,
                             std::string_view itemName, std::string_view paramStr);
    ~BaseRotatingFileSinkImpl() override = default;

    [[nodiscard]] std::vector<std::string> get_file_list();

protected:
    virtual void init_file_queue() = 0;
    void set_max_files_it(uint32_t maxFiles);
    [[nodiscard]] uint32_t max_files_it() const;
    void push_back_file(std::string_view file);
    void rotate(std::string_view newFile);
    void delete_overflow_file();

private:
    std::atomic<uint32_t> _maxFiles{0};
    std::string _itemName;
    std::deque<std::string> _fileQue;
};

}  // namespace logging

#endif  // LOGGING_INTERNAL_SINKS_BASE_ROTATING_FILE_SINK_IMPL_H
