#include "logging/logger.h"

#include <cstdarg>

#include "logging/log_record.h"
#include "logging/log_source.h"

namespace logging {

Logger::Logger(std::string_view name) : m_name(name) {}

void Logger::log(LogLevel level, const char *format, ...) {}
void Logger::log(const char *file, int line, const char *func, LogLevel level, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    LogRecord recorder(this->m_name, level, format, ap, LogSrc(file, line, func));
    va_end(ap);

    for (auto sink : m_sinkList) {
        sink->log(recorder);
    }
}
}  // namespace logging
