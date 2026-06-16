#ifndef COMMON_DEBUG_DEBUG_LEVEL_H
#define COMMON_DEBUG_DEBUG_LEVEL_H

#ifndef __cplusplus
extern "C" {
#endif
typedef enum {
    DEBUG_LVL_TRACE = 0,
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
        case DEBUG_LVL_TRACE:
            return "T";
        case DEBUG_LVL_DEBUG:
            return "D";
        case DEBUG_LVL_INFO:
            return "I";
        case DEBUG_LVL_WARN:
            return "W";
        case DEBUG_LVL_ERR:
            return "E";
        case DEBUG_LVL_FATAL:
            return "F";
        case DEBUG_LVL_OFF:
            return "O";
        default:
            return "UNKNOWN";
    }
}
#ifndef __cplusplus
}
#endif
#endif  // COMMON_DEBUG_DEBUG_LEVEL_H
