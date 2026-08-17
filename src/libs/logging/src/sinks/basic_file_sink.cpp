#include "logging/sinks/basic_file_sink.h"

#include <memory>

#include "logging/sinks/sink_base.h"
#include "sinks/internal/basic_file_sink_impl.h"

namespace origin::logging {

BasicFileSink::BasicFileSink() : SinkBase(std::make_unique<BasicFileSinkImpl>()) {}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite)
    : SinkBase(std::make_unique<BasicFileSinkImpl>(file, overwrite))
{
}

std::string BasicFileSink::file() const
{
    throw_if_pimpl_null();
    return dynamic_cast<const BasicFileSinkImpl *>(_pImpl.get())->file();
}

}  // namespace origin::logging
