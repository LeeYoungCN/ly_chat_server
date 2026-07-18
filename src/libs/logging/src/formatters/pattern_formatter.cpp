#include "logging/formatters/pattern_formatter.h"

#include <format>
#include <iterator>
#include <memory>
#include <string>

#include "common/types/date_time_types.h"
#include "logging/log_level.h"
#include "utils/date_time_utils.h"
#include "utils/filesystem_utils.h"

namespace logging {

using namespace utils::date_time;
using namespace utils::filesystem;

PatternFormatter::PatternFormatter(std::string_view pattern) : _pattern(pattern) {}

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

void PatternFormatter::format_time(const details::LogMsg& logMsg, std::string& logContent)
{
    DateTimeSt datetime = timestamp_to_date_time(logMsg.timestamp);
    std::format_to(std::back_insert_iterator(logContent),
                   "{:04}-{:02}-{:02} {:02}:{:02}:{:02}.{:03}",
                   datetime.year,
                   datetime.month,
                   datetime.day,
                   datetime.hour,
                   datetime.minute,
                   datetime.second,
                   datetime.millis);
}

void PatternFormatter::log_msg_to_content(char symbol, const details::LogMsg& logMsg,
                                          std::string& logContent)
{
    switch (symbol) {
        case 'd':  // datetime
            format_time(logMsg, logContent);
            break;
        case 'n':  // logger name
            logContent.append(logMsg.loggerName);
            break;
        case 'l':  // lower level string
            logContent.append(log_level_to_string(logMsg.level, false));
            break;
        case 'L':  // upper level string
            logContent.append(log_level_to_string(logMsg.level));
            break;
        case 's':  // file name
            logContent.append(
                (logMsg.source.file.empty() ? "Filename" : get_filename(logMsg.source.file)));
            break;
        case 'g':  // file path
            logContent.append(logMsg.source.file.empty() ? "FilePath" : logMsg.source.file);
            break;
        case '#':  // lineNumber
            logContent.append(std::to_string(logMsg.source.line));
            break;
        case '!':  // function name
            logContent.append(logMsg.source.func.empty() ? "Function" : logMsg.source.func);
            break;
        case 't':  // thread id
            logContent.append(std::to_string(logMsg.threadId));
            break;
        case 'P':  // process id
            logContent.append(std::to_string(logMsg.procId));
            break;
        case 'v':  // message
            logContent.append(logMsg.data);
            break;
        case '%':
            logContent.push_back('%');
            break;
        default:
            logContent.push_back('%');
            logContent.push_back(symbol);
            break;
    }
}

std::unique_ptr<Formatter> PatternFormatter::clone() const
{
    return std::make_unique<PatternFormatter>(_pattern);
}

}  // namespace logging
