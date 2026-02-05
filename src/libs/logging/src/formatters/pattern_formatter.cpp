#include "logging/formatters/pattern_formatter.h"

#include <string>
#include <string_view>

#include "common/utils/date_time_utils.h"
#include "common/utils/filesystem_utils.h"
#include "common/utils/process_utils.h"
#include "logging/log_level.h"

namespace {
constexpr std::string_view DEFAULT_TIME_PATTERN = "%Y-%m-%d %H:%M:%S.%3f";
}

namespace logging {
using namespace common::process;
using namespace common::date_time;
using namespace common::filesystem;

PatternFormatter::PatternFormatter(std::string pattern) : _pattern(std::move(pattern)) {}

void PatternFormatter::format(const details::LogMsg& logMsg, std::string& content)
{
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

void PatternFormatter::log_msg_to_content(char symbol, const details::LogMsg& logMsg, std::string& content)
{
    switch (symbol) {
        case 'd':  // datetime
            content.append(FormatTimeString(logMsg.timeStamp, DEFAULT_TIME_PATTERN));
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

}  // namespace logging
