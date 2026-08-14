#ifndef TEST_LOGGING_TEST_UTILS_TID_MSG_SINK_H
#define TEST_LOGGING_TEST_UTILS_TID_MSG_SINK_H

#include <map>

#include "base_test_sink.h"
#include "common/types/thread_types.h"
#include "logging/log_msg.h"

namespace test::test_logging {
class TidMsgMapSink : public BaseTestSink {
public:
    TidMsgMapSink() = default;
    ~TidMsgMapSink() override = default;

protected:
    void log_it(const LogMsg& logMsg) override;
    void flush_it() override {}

    const std::map<ThreadId, LogMsg>& buffer();

private:
    std::map<ThreadId, LogMsg> _buffer;
};
}  // namespace test::test_logging
#endif  // TEST_LOGGING_TEST_UTILS_TID_MSG_SINK_H
