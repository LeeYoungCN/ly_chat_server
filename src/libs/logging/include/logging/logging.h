#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

#include "logging/log_level.h"

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

typedef struct sink_st sink_st;

typedef enum {
    CONSOLE_STDOUT = 0,
    CONSOLE_STDERR
} ConsoleType;

sink_st *logging_get_stdout_sink(ConsoleType type);

sink_st *logging_get_basic_file_sink(const char *file, bool overwrite);

void logging_add_sink(sink_st *sink);

void logging_set_level(LogLevel level);

void logging_flush();

void logging_debug(const char *file, int line, const char *func, const char *format, ...);

void logging_info(const char *file, int line, const char *func, const char *format, ...);

void logging_warn(const char *file, int line, const char *func, const char *format, ...);

void logging_error(const char *file, int line, const char *func, const char *format, ...);

void logging_fatal(const char *file, int line, const char *func, const char *format, ...);

#define LOGGING_DEBUG(fmt, ...) logging_debug(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#define LOGGING_INFO(fmt, ...)  logging_debug(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#define LOGGING_WARN(fmt, ...)  logging_warn(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#define LOGGING_ERR(fmt, ...)   logging_error(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#define LOGGING_FATAL(fmt, ...) logging_fatal(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // LOGGING_LOGGING_H
