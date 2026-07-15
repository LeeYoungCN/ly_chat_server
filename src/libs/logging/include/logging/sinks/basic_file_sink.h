#pragma once
#ifndef LOGGINGL_LOG_BASIC_FILE_SINK_H
#define LOGGINGL_LOG_BASIC_FILE_SINK_H

#include <string>
#include <string_view>

#include "logging/details/log_msg.h"
#include "logging/sinks/sink.h"
#include "utils/file_writer.h"

namespace logging {
class BasicFileSink : public Sink {
public:
    BasicFileSink();
    ~BasicFileSink() override;
    explicit BasicFileSink(std::string_view file, bool overwrite = true);

    [[nodiscard]] const std::string& file() const;
    [[nodiscard]] const std::string& directory() const;
    [[nodiscard]] const std::string& filename() const;
    [[nodiscard]] const std::string& filename_stem() const;
    [[nodiscard]] const std::string& extention() const;

protected:
    BasicFileSink(std::string_view file, bool overwrite, std::string_view paramStr);
    void log_it(const details::LogMsg& logMsg) override;
    void flush_it() override;
    void sink_it(std::string_view message);
    utils::filesystem::FileWriter& file_writer_it();

private:
    std::string _file;
    std::string _directory;
    std::string _filename;
    std::string _filename_stem;
    std::string _extention;

    utils::filesystem::FileWriter _fileWriter;
};

}  // namespace logging

#endif  // LOGGINGL_LOG_BASIC_FILE_SINK_H
