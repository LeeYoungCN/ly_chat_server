#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <stdint.h>

#include "common/types/error_code_types.h"
#include "common/types/logging_types.h"

typedef struct LogSinkSt LogSinkSt;

typedef enum {
    CONSOLE_STDOUT = 0,
    CONSOLE_STDERR
} ConsoleType;

LogSinkSt *logging_get_console_sink(ConsoleType type);

LogSinkSt *logging_get_basic_file_sink(const char *file, bool rewrite);

void logging_register_sink(LogSinkSt *sink);

void logging_set_level(LogLevel level);

ErrorCode logging_init();

void logging_close();

void logging_log(const char *file, int line, const char *func, LogLevel level, const char *format, ...);

#define LOGGING_LOG(level, fmt, ...)                                                          \
    do {                                                                                      \
        logging_log(__FILE__, __LINE__, __FUNCTION__, level, fmt __VA_OPT__(, ) __VA_ARGS__); \
    } while (0)

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // LOGGING_LOGGING_H
