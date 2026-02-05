#include "logging/log_level.h"

#include <string_view>
#include <unordered_map>

namespace logging {

struct LogLvlStrInfo
{
    std::string_view upper;
    std::string_view lower;
};

std::string_view log_level_to_string(LogLevel level, bool upper)
{
    static std::unordered_map<LogLevel, LogLvlStrInfo> LOG_STR_MAP = {
        {LogLevel::DEBUG, {"DEBUG", "debug"}},
        {LogLevel::INFO, {"INFO", "info"}},
        {LogLevel::WARN, {"WARN", "warn"}},
        {LogLevel::ERROR, {"ERROR", "error"}},
        {LogLevel::FATAL, {"FATAL", "fatal"}},
        {LogLevel::OFF, {"OFF", "off"}}
        };

    return upper ? LOG_STR_MAP[level].upper : LOG_STR_MAP[level].lower;
}
}  // namespace logging
