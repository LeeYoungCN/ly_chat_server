#ifndef LOGGING_C_LOGGING_C_H
#define LOGGING_C_LOGGING_C_H

#include <cstdint>
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
#include <stdio.h>

#include "common/compiler/macros.h"

typedef struct LoggerSt LoggerSt;
typedef struct SinkSt SinkSt;
typedef struct FormatterSt FormatterSt;
typedef struct TaskPoolSt TaskPoolSt;

#define LOGGING_LOGGER_ROOT_NAME               "__root_logger__"
#define LOGGING_FORMATTER_DEFAULT_PATTERN      "[%d][%L][%s:%#]: %v"
#define LOGGING_FORMATTER_DEFAULT_TIME_PATTERN "%Y-%m-%d %H:%M:%S.%3f"
#define LOGGING_THREAD_POOL_DEFAULT_CAPACITY   (4096)
#define LOGGING_THREAD_POOL_DEFAULT_THREAD_CNT (1)

typedef enum {
    LOGGING_LEVEL_DEBUG,
    LOGGING_LEVEL_INFO,
    LOGGING_LEVEL_WARN,
    LOGGING_LEVEL_ERROR,
    LOGGING_LEVEL_FATAL,
    LOGGING_LEVEL_OFF
} LoggingLevel;

#pragma region Logger
COMMON_API LoggerSt *logging_create_logger(const char *name, const SinkSt *const sinks[],
                                           uint32_t count);
COMMON_API LoggerSt *logging_create_async_logger(const char *name, const SinkSt *const sinks[],
                                                 uint32_t count, const TaskPoolSt *taskPool);
COMMON_API void logging_destroy_logger(LoggerSt *logger);

COMMON_API const char *logging_logger_name(LoggerSt *logger);

COMMON_API void logging_logger_set_level(LoggerSt *logger, LoggingLevel level);
COMMON_API bool logging_logger_should_log(LoggerSt *logger, LoggingLevel level);
COMMON_API LoggingLevel logging_logger_level(LoggerSt *logger);

COMMON_API void logging_logger_flush_on(LoggerSt *logger, LoggingLevel level);
COMMON_API bool logging_logger_should_flush(LoggerSt *logger, LoggingLevel level);
COMMON_API LoggingLevel logging_logger_flush_level(LoggerSt *logger);

COMMON_API void logging_logger_set_pattern(LoggerSt *logger, const char *pattern,
                                           const char *timePattern);
COMMON_API void logging_logger_set_formatter(LoggerSt *logger, const FormatterSt *formatter);

COMMON_API void logging_logger_flush(LoggerSt *logger);

COMMON_API void logging_logger_log(LoggerSt *logger, const char *file, int line, const char *func,
                                   LoggingLevel level, const char *format, ...);
#pragma endregion

#pragma region Sink
COMMON_API SinkSt *logging_create_stdout_sink(FILE *file);
COMMON_API SinkSt *logging_create_basic_file_sink(const char *file, bool overwrite);

COMMON_API void logging_detroy_sink(SinkSt *sink);

COMMON_API void logging_sink_set_level(SinkSt *sink, LoggingLevel level);
COMMON_API bool logging_sink_should_log(SinkSt *sink, LoggingLevel level);
COMMON_API LoggingLevel logging_sink_level(SinkSt* sink);

COMMON_API void logging_sink_set_pattern(SinkSt *sink, const char *pattern,
                                         const char *timePattern);
COMMON_API void logging_sink_set_formatter(SinkSt *sink, const FormatterSt *formatter);
#pragma endregion

#pragma region Formatter
COMMON_API FormatterSt *logging_create_pattern_formatter(const char *pattern,
                                                         const char *timePattern);
COMMON_API void logging_destroy_formatter(FormatterSt *formatter);
#pragma endregion

#pragma region Task pool
COMMON_API TaskPoolSt *logging_create_task_pool(uint32_t capacity, uint32_t threadCnt);
COMMON_API void logging_destroy_task_pool(TaskPoolSt *taskPool);
#pragma endregion

#pragma region Root logger
COMMON_API LoggerSt *logging_root_logger();
COMMON_API void logging_set_root_logger(LoggerSt *logger);

COMMON_API void logging_set_level(LoggingLevel level);
COMMON_API bool logging_should_log(LoggingLevel level);
COMMON_API LoggingLevel logging_level();

COMMON_API void logging_flush_on(LoggingLevel level);
COMMON_API bool logging_should_flush(LoggingLevel level);
COMMON_API LoggingLevel logging_flush_level();

COMMON_API void logging_set_pattern(const char *pattern, const char *timePattern);
COMMON_API void logging_set_formatter(const FormatterSt *formatter);

COMMON_API void logging_flush();

COMMON_API void logging_log(const char *file, int line, const char *func, LoggingLevel level,
                            const char *format, ...);
#pragma endregion

#pragma region Container

#pragma endregion

#pragma region Module manager

#pragma endregion

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

#endif  // LOGGING_C_LOGGING_C_H
