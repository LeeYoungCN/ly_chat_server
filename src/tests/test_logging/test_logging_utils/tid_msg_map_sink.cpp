#include "tid_msg_map_sink.h"

#include <mutex>

using namespace logging;
using namespace logging::details;

namespace test::test_logging {
void TidMsgMapSink::log_it(const LogMsg& logMsg)
{
    _buffer[logMsg.threadId] = logMsg;
}

const std::map<ThreadId, LogMsg>& TidMsgMapSink::buffer()
{
    std::lock_guard<std::mutex> lock(_sinkMtx);
    return _buffer;
}
}  // namespace test::test_logging
