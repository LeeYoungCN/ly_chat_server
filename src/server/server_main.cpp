#include "common/debug/debug_log.h"

int main(int argc, char *argv[])
{
    set_debug_log_level(DEBUG_LVL_DEBUG);
    for (int i = 0; i < argc; i++) {
        DEBUG_LOG_INFO("argv[%d]: %s", i, argv[i]);
    }
    return 0;
}
