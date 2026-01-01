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

#include <string>

#include "common/compiler/macros.h"
#include "common/types/thread_types.h"

#if PLATFORM_WINDOWS
#include <windows.h>
#elif PLATFORM_LINUX
#include <sys/syscall.h>
#include <unistd.h>
#elif PLATFORM_MACOS
#include <pthread.h>
#else
#error "Unsupport system"
#endif

namespace {

thread_local std::string g_threadName;

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

extern "C" {

ThreadId GetCurrThreadId()
{
    static thread_local ThreadId tid = GetCurrentThreadIdInternal();
    return tid;
}

void SetCurrThreadName(const char *name)
{
    g_threadName = name;
}

const char *GetCurrThreadName()
{
    return g_threadName.c_str();
}

}  // namespace common::utils::thread
