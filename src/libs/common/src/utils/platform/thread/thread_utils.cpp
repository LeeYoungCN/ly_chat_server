/**
 * @file thread_utils.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2025-08-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "common/utils/thread_utils.h"

#include <threads.h>
#include <unistd.h>

#include "common/compiler/macros.h"
#include "common/types/thread_types.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <sys/syscall.h>
#elif PLATFORM_MACOS
#include <pthread.h>
#else
#error "Unsupport system"
#endif

namespace {
using namespace common::types::thread;

thread_local ThreadName g_threadName;

ThreadId GetCurrentThreadIdInternal()
{
#if PLATFORM_WINDOWS
    return static_cast<ThreadId>(GetCurrentThreadId());
#elif PLATFORM_LINUX
    return static_cast<ThreadId>(syscall(SYS_gettid));
#elif PLATFORM_MACOS
    uint64_t tid;
    pthread_threadid_np(nullptr, &tid);
    return static_cast<ThreadId>(tid);
#else
    return 0;
#endif
}
}  // namespace

using namespace common::types::thread;

namespace common::utils::thread {

ThreadId GetCurrentThreadId()
{
    static thread_local ThreadId tid = GetCurrentThreadIdInternal();
    return tid;
}

void SetCurrentThreadName(const ThreadName &name)
{
    g_threadName = name;
}

ThreadName GetCurrentThreadName()
{
    return g_threadName;
}

}  // namespace common::utils::thread
