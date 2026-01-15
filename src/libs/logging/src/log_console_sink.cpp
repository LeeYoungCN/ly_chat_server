#include "logging/log_console_sink.h"

#include <cstdio>

#include "common/common_error_code.h"
#include "common/types/error_code_types.h"
#include "logging/log_record.h"
#include "logging/logging.h"

namespace logging {
LogConsoleSink::LogConsoleSink(ConsoleType type) : m_type(type)
{
    if (m_type == CONSOLE_STDERR) {
        m_stream = stderr;
    } else {
        m_stream = stdout;
    }
}

ErrorCode LogConsoleSink::init()
{
    fflush(m_stream);
    return ERR_COMM_SUCCESS;
}

void LogConsoleSink::close()
{
    fflush(m_stream);
}

void LogConsoleSink::log(const LogRecord& logRecord)
{
    auto logMsg = format(logRecord);
    fprintf(m_stream, "%s", logMsg.c_str());
}
}  // namespace logging
