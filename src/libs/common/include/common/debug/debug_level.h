#ifndef COMMON_DEBUG_DEBUG_LEVEL_H
#define COMMON_DEBUG_DEBUG_LEVEL_H

#ifndef __cplusplus
extern "C" {
#endif
typedef enum {
    DEBUG_LVL_DEBUG,
    DEBUG_LVL_INFO,
    DEBUG_LVL_WARN,
    DEBUG_LVL_ERR,
    DEBUG_LVL_FATAL,
    DEBUG_LVL_OFF
} DebugLevel;

static inline const char* get_debug_log_lvl_str(DebugLevel level)
{
    switch (level) {
        case DEBUG_LVL_DEBUG:
            return "DEBUG";
        case DEBUG_LVL_INFO:
            return "INFO";
        case DEBUG_LVL_WARN:
            return "WARN";
        case DEBUG_LVL_ERR:
            return "ERROR";
        case DEBUG_LVL_FATAL:
            return "FATAL";
        case DEBUG_LVL_OFF:
            return "OFF";
        default:
            return "UNKNOWN";
    }
}
#ifndef __cplusplus
}
#endif
#endif  // COMMON_DEBUG_DEBUG_LEVEL_H
