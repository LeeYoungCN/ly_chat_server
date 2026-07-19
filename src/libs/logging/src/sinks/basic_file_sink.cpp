#include "logging/sinks/basic_file_sink.h"

#include <memory>

#include "internal/sinks/basic_file_sink_impl.h"
#include "logging/sinks/base_sink.h"

namespace logging {

BasicFileSink::BasicFileSink() : BaseSink(std::make_unique<BasicFileSinkImpl>()) {}

BasicFileSink::BasicFileSink(std::string_view file, bool overwrite)
    : BaseSink(std::make_unique<BasicFileSinkImpl>(file, overwrite))
{
}

std::string BasicFileSink::file() const
{
    throw_if_pimpl_null();
    return dynamic_cast<const BasicFileSinkImpl*>(_pImpl.get())->file();
}

}  // namespace logging
