#ifndef LOGGING_SINKS_BASIC_ROTATING_FILE_SINK_H
#define LOGGING_SINKS_BASIC_ROTATING_FILE_SINK_H

#include <cstdint>
#include <deque>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "logging/sinks/sink.h"
#include "utils/file_writer.h"

namespace logging {
enum class RotatingPolicy {
    OPEN_NEW = 0,
    RENAME
};

class BaseRotatingFileSink : public Sink {
public:
    BaseRotatingFileSink(std::string_view logFile, uint32_t maxFiles, RotatingPolicy rotatingPolicy,
                         std::string_view itemName, std::string_view paramStr);
    ~BaseRotatingFileSink() override;

    std::string log_file();
    std::vector<std::string> get_file_list();

protected:
    std::shared_ptr<utils::filesystem::FileWriter> _fileWriter;
    void log_it(const details::LogMsg& logMsg) override = 0;
    void sink_it(std::string_view message);
    void flush_it() override;

    virtual void init_file_queue() = 0;
    void push_back_file(std::string_view file);
    void rotate(std::string_view newFile);
    void delete_overflow_file();

    [[nodiscard]] const std::string& base_file_it() const;
    [[nodiscard]] const std::string& directory_it() const;
    [[nodiscard]] const std::string& filename_stem_it() const;
    [[nodiscard]] const std::string& extention_it() const;

private:
    void rotate_open_new(std::string_view newFile);
    void rotate_replace(std::string_view newFile);

    const std::string _baseFile;
    const std::string _directory;
    const std::string _fileStem;
    const std::string _extention;
    uint32_t _maxFiles{0};
    std::string _itemName;

    RotatingPolicy _policy{RotatingPolicy::OPEN_NEW};

    std::deque<std::string> _fileQue;
};

}  // namespace logging

#endif  // LOGGING_SINKS_BASIC_ROTATING_FILE_SINK_H
