#ifndef TEST_LOGGING_TEST_UTILS_TID_MSG_SINK_H
#define TEST_LOGGING_TEST_UTILS_TID_MSG_SINK_H

#include <map>

#include "common/types/thread_types.h"
#include "logging/details/log_msg.h"
#include "logging/sinks/sink.h"

namespace test::test_logging {
class TidMsgMapSink : public logging::Sink {
public:
    TidMsgMapSink() = default;
    ~TidMsgMapSink() override = default;

    void log(const logging::details::LogMsg& logMsg) override;
    void flush() override {}

    const std::map<ThreadId, logging::details::LogMsg>& buffer();

private:
    std::map<ThreadId, logging::details::LogMsg> _buffer;
};
}  // namespace test::test_logging
#endif  // TEST_LOGGING_TEST_UTILS_TID_MSG_SINK_H
