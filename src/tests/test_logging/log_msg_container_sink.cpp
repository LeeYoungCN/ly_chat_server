#include "log_msg_container_sink.h"

#include <mutex>

using namespace logging;
using namespace logging::details;

namespace test::test_logging {
void LogMsgContainer::log(const LogMsg& logMsg)
{
    std::lock_guard<std::mutex> lock(_sinkMtx);
    _buffer[logMsg.threadId] = logMsg;
}

const std::map<ThreadId, LogMsg>& LogMsgContainer::buffer()
{
    std::lock_guard<std::mutex> lock(_sinkMtx);
    return _buffer;
}
}  // namespace test::test_logging
