#pragma once
#ifndef LOGGINGL_INTERNAL_SINKS_BASIC_FILE_SINK_IMPL_H
#define LOGGINGL_INTERNAL_SINKS_BASIC_FILE_SINK_IMPL_H

#include <string>
#include <string_view>

#include "internal/sinks/sink_impl_base.h"
#include "logging/details/log_msg.h"
#include "utils/file_writer.h"

namespace logging {
class BasicFileSinkImpl : public SinkImplBase {
public:
    BasicFileSinkImpl();
    ~BasicFileSinkImpl() override = default;
    explicit BasicFileSinkImpl(std::string_view file, bool overwrite = true);

    [[nodiscard]] const std::string& file() const;

protected:
    BasicFileSinkImpl(std::string_view file, bool overwrite, std::string_view paramStr);
    void log_it(const details::LogMsg& logMsg) override;
    void flush_it() override;
    void sink_it(std::string_view message);

    const std::string _file;
    const std::string _directory;
    const std::string _filename;
    const std::string _filenameStem;
    const std::string _extention;
    bool _overwrite{false};
    utils::filesystem::FileWriter _fileWriter;
};

}  // namespace logging

#endif  // LOGGINGL_INTERNAL_SINKS_BASIC_FILE_SINK_IMPL_H
