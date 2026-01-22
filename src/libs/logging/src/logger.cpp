#include "logging/logger.h"

#include <cstdarg>

namespace logging {

Logger::Logger(std::string_view name) : m_name(name) {}

void Logger::set_level(LogLevel level)
{
    m_level = level;
}

void Logger::set_name(std::string_view name)
{
    this->m_name = name;
}

std::string_view Logger::get_name() const
{
    return this->m_name;
}

void Logger::add_sink(const std::shared_ptr<Sink>& sink)
{
    m_sinkList.emplace_back(sink);
}

bool Logger::should_log(LogLevel level) const
{
    return level >= m_level;
}

void Logger::log(LogSource source, LogLevel level, std::string message)
{
    if (!should_log(level)) {
        return;
    }

    LogRecord logRecord(this->m_name, level, std::move(message), source);

    for (const auto& sink : m_sinkList) {
        sink->log(logRecord);
    }

    if (level >= LogLevel::LOG_LVL_ERR) {
        flush();
    }
}

void Logger::flush()
{
    for (const auto& sink : m_sinkList) {
        sink->flush();
    }
}

Logger::~Logger()
{
    flush();
    m_sinkList.clear();
};

}  // namespace logging
