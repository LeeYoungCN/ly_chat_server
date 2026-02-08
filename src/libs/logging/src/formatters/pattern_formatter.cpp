#include "logging/formatters/pattern_formatter.h"

#include <memory>
#include <string>
#include <string_view>

#include "common/utils/date_time_utils.h"
#include "common/utils/filesystem_utils.h"
#include "common/utils/process_utils.h"
#include "logging/details/common.h"
#include "logging/log_level.h"

namespace logging {
using namespace common::process;
using namespace common::date_time;
using namespace common::filesystem;

PatternFormatter::PatternFormatter()
    : PatternFormatter(FORMATTER_DEFAULT_PATTERN, FORMATTER_DEFAULT_TIME_PATTERN)
{
}

PatternFormatter::PatternFormatter(std::string_view pattern)
    : PatternFormatter(pattern, FORMATTER_DEFAULT_TIME_PATTERN)
{
}

PatternFormatter::PatternFormatter(std::string_view pattern, std::string_view timePattern)
    : _pattern(pattern), _timePattern(timePattern)
{
}

void PatternFormatter::format(const details::LogMsg& logMsg, std::string& content)
{
    constexpr uint32_t LOG_CONTENT_DEFAULT_LEN = 256;
    content.reserve(LOG_CONTENT_DEFAULT_LEN);
    bool needTrans = false;
    for (const char& c : _pattern) {
        if (needTrans) {
            log_msg_to_content(c, logMsg, content);
            needTrans = false;
        } else if (c == '%') {
            needTrans = true;
        } else {
            content.push_back(c);
        }
    }
}

void PatternFormatter::log_msg_to_content(char symbol, const details::LogMsg& logMsg,
                                          std::string& content)
{
    switch (symbol) {
        case 'd':  // datetime
            content.append(FormatTimeString(logMsg.timeStamp, _timePattern));
            break;
        case 'n':  // logger name
            content.append(logMsg.loggerName);
            break;
        case 'l':  // lower level string
            content.append(log_level_to_string(logMsg.level, false));
            break;
        case 'L':  // upper level string
            content.append(log_level_to_string(logMsg.level));
            break;
        case 's':  // file name
            content.append(GetBaseName(logMsg.source.file));
            break;
        case 'g':  // file path
            content.append(logMsg.source.file);
            break;
        case '#':  // lineNumber
            content.append(std::to_string(logMsg.source.line));
            break;
        case '!':  // function name
            content.append(logMsg.source.func);
            break;
        case 't':  // thread id
            content.append(std::to_string(logMsg.threadId));
            break;
        case 'P':  // process id
            content.append(std::to_string(GetCurrProcessId()));
            break;
        case 'v':  // message
            content.append(logMsg.data);
            break;
        case '%':
            content.push_back('%');
            break;
        default:
            content.push_back('%');
            content.push_back(symbol);
            break;
    }
}

std::unique_ptr<Formatter> PatternFormatter::clone() const
{
    return std::make_unique<PatternFormatter>(_pattern, _timePattern);
}

}  // namespace logging
