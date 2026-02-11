#ifndef TEST_LOGGING_LOG_MSG_CONTAINER_SINK_H
#define TEST_LOGGING_LOG_MSG_CONTAINER_SINK_H

#include <map>

#include "common/types/thread_types.h"
#include "logging/details/log_msg.h"
#include "logging/sinks/sink.h"

namespace test::test_logging {
class LogMsgContainer : public logging::Sink {
public:
    LogMsgContainer() = default;
    ~LogMsgContainer() override = default;

    void log(const logging::details::LogMsg& logMsg) override;
    void flush() override {}

    const std::map<ThreadId, logging::details::LogMsg>& buffer();

private:
    std::map<ThreadId, logging::details::LogMsg> _buffer;
};
}  // namespace test::test_logging
#endif  // TEST_LOGGING_LOG_MSG_CONTAINER_SINK_H
