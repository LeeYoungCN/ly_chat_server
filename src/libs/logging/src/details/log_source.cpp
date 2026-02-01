#include "logging/details/log_source.h"

namespace logging::details {
LogSource::LogSource(std::string_view filePath, int lineNumber, std::string_view funcName)
    : file(filePath), line(lineNumber), func(funcName)
{
}

[[nodiscard]] bool LogSource::IsValid() const
{
    return (!file.empty() && line != INVALID_LINE_NUM);
}
}  // namespace logging::details
