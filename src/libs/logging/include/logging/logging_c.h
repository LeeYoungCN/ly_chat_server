#ifndef LOGGING_LOGGING_C_H
#define LOGGING_LOGGING_C_H

#include <cstdint>
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
#include <stdio.h>

typedef struct sink_st sink_st;
typedef struct logger_st logger_st;

typedef enum {
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL,
    LOG_LEVEL_OFF
} LoggingLevel;

logger_st *logging_logger(const char *name, sink_st* sinks[], uint32_t count);

sink_st *logging_stdout_sink(FILE *file);

sink_st *logging_basic_file_sink(const char *file, bool overwrite);

void logging_set_level(LoggingLevel level);

LoggingLevel logging_level();

void logging_set_pattern(const char *pattern, const char* timePattern);

void logging_flush();

void logging_log(const char *file, int line, const char *func, LoggingLevel level, const char *format, ...);

void logging_debug(const char *file, int line, const char *func, const char *format, ...);

void logging_info(const char *file, int line, const char *func, const char *format, ...);

void logging_warn(const char *file, int line, const char *func, const char *format, ...);

void logging_error(const char *file, int line, const char *func, const char *format, ...);

void logging_fatal(const char *file, int line, const char *func, const char *format, ...);
#ifdef __cplusplus
}
#endif  // __cplusplus

#define LOGGING_DEBUG(fmt, ...) \
    logging_debug(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#define LOGGING_INFO(fmt, ...) \
    logging_info(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#define LOGGING_WARN(fmt, ...) \
    logging_warn(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#define LOGGING_ERR(fmt, ...) \
    logging_error(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);
#define LOGGING_FATAL(fmt, ...) \
    logging_fatal(__FILE__, __LINE__, __FUNCTION__, fmt __VA_OPT__(, ) __VA_ARGS__);

#endif  // LOGGING_LOGGING_C_H
