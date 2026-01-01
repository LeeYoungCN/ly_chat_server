#include "common/debug/debug_log.h"

int main(int argc, char *argv[])
{
    SetDebugLogLevel(LOG_LVL_DEBUG);
    for (int i = 0; i < argc; i++) {
        DEBUG_LOG_INFO("argv[%d]: %s", i, argv[i]);
    }
    return 0;
}
