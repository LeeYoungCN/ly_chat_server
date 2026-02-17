#include "logging/log_level.h"

#include <string_view>
#include <unordered_map>

namespace logging {

struct LogLvlStrInfo {
    std::string_view full;
    std::string_view abbr;
};

std::string_view log_level_to_string(LogLevel level, bool full)
{
    static std::unordered_map<LogLevel, LogLvlStrInfo> LOG_STR_MAP = {
        {LogLevel::DEBUG, {"DEBUG", "D"}},
        {LogLevel::INFO, {"INFO", "I"}},
        {LogLevel::WARN, {"WARN", "W"}},
        {LogLevel::ERROR, {"ERROR", "E"}},
        {LogLevel::FATAL, {"FATAL", "F"}},
        {LogLevel::OFF, {"OFF", "O"}}};

    return full ? LOG_STR_MAP[level].full : LOG_STR_MAP[level].abbr;
}
}  // namespace logging
