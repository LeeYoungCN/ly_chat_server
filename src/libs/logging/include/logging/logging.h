#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include "common/types/logging_types.h"

typedef struct LogSink LogSink;

typedef enum {
    CONSOLE_STDOUT = 0,
    CONSOLE_STDERR
} ConsoleType;

LogSink *logging_get_console_sink(LogLevel level, ConsoleType type);
LogSink *logging_get_file_sink(LogLevel level, const char *file);

void logging_register_sink(LogSink *sink);

void logging_init();
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
