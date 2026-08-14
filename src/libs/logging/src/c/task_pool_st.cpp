#include <memory>

#include "c/common_c.h"
#include "internal/task_pool.h"
#include "logging/c/logging_c.h"

using namespace logging;

extern "C" {
TaskPoolSt *logging_create_task_pool(uint32_t capacity, uint32_t threadCnt)
{
    return new TaskPoolSt(std::make_shared<TaskPool>(capacity, threadCnt));
}

void logging_destroy_task_pool(TaskPoolSt *taskPool)
{
    if (taskPool == nullptr) {
        return;
    }
    taskPool->ptr.reset();
    delete taskPool;
}
}
