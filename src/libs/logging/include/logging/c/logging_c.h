#ifndef ORIGIN_LOGGING_C_LOGGING_C_H
#define ORIGIN_LOGGING_C_LOGGING_C_H

#include <cstdint>
#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
#include <stdio.h>

typedef struct LoggerSt LoggerSt;
typedef struct SinkSt SinkSt;
typedef struct FormatterSt FormatterSt;
typedef struct TaskPoolSt TaskPoolSt;

#define ORIGIN_LOGGER_ROOT_NAME               "__root_logger__"
#define ORIGIN_FORMATTER_DEFAULT_PATTERN      "[%d][%L][%s:%#]: %v"
#define ORIGIN_THREAD_POOL_DEFAULT_CAPACITY   (4096)
#define ORIGIN_THREAD_POOL_DEFAULT_THREAD_CNT (1)

typedef enum {
    ORIGIN_LOG_LEVEL_TRACE = 0,
    ORIGIN_LOG_LEVEL_DEBUG,
    ORIGIN_LOG_LEVEL_INFO,
    ORIGIN_LOG_LEVEL_WARN,
    ORIGIN_LOG_LEVEL_ERROR,
    ORIGIN_LOG_LEVEL_FATAL,
    ORIGIN_LOG_LEVEL_OFF
} OriginLogLevel;

#pragma region Logger
LoggerSt *origin_create_sync_logger(const char *name, const SinkSt *const sinks[], uint32_t count);
LoggerSt *origin_create_async_logger(const char *name, const SinkSt *const sinks[], uint32_t count,
                                     const TaskPoolSt *taskPool);
void origin_destroy_logger(LoggerSt *logger);

const char *origin_logger_name(LoggerSt *logger);

void origin_logger_set_level(LoggerSt *logger, OriginLogLevel level);
bool origin_logger_should_log(LoggerSt *logger, OriginLogLevel level);
OriginLogLevel origin_logger_level(LoggerSt *logger);

void origin_logger_flush_on(LoggerSt *logger, OriginLogLevel level);
bool origin_logger_should_flush(LoggerSt *logger, OriginLogLevel level);
OriginLogLevel origin_logger_flush_level(LoggerSt *logger);

void origin_logger_set_pattern(LoggerSt *logger, const char *pattern);
void origin_logger_set_formatter(LoggerSt *logger, const FormatterSt *formatter);

void origin_logger_flush(LoggerSt *logger);

void origin_logger_log(LoggerSt *logger, const char *file, int line, const char *func,
                       OriginLogLevel level, const char *format, ...);
#pragma endregion

#pragma region Sink
SinkSt *origin_create_stdout_sink(FILE *file);
SinkSt *origin_create_basic_file_sink(const char *file, bool overwrite);
SinkSt *origin_create_daily_file_sink(const char *file, uint32_t hour, uint32_t minute,
                                      uint32_t maxFiles, bool overwrite);
SinkSt *origin_create_rotating_file_sink(const char *file, uint32_t maxFileSize, uint32_t maxFiles,
                                         bool rotateOnOpen);

void origin_detroy_sink(SinkSt *sink);

void origin_sink_set_level(SinkSt *sink, OriginLogLevel level);
bool origin_sink_should_log(SinkSt *sink, OriginLogLevel level);
OriginLogLevel origin_sink_level(SinkSt *sink);

void origin_sink_set_pattern(SinkSt *sink, const char *pattern);
void origin_sink_set_formatter(SinkSt *sink, const FormatterSt *formatter);
#pragma endregion

#pragma region Formatter
FormatterSt *origin_create_pattern_formatter(const char *pattern);
void origin_destroy_formatter(FormatterSt *formatter);
#pragma endregion

#pragma region Task pool
TaskPoolSt *origin_create_task_pool(uint32_t capacity, uint32_t threadCnt);
void origin_destroy_task_pool(TaskPoolSt *taskPool);
#pragma endregion

#pragma region Root logger
LoggerSt *origin_root_logger();
void origin_set_root_logger(LoggerSt *logger);

void origin_set_level(OriginLogLevel level);
bool origin_should_log(OriginLogLevel level);
OriginLogLevel origin_level();

void origin_flush_on(OriginLogLevel level);
bool origin_should_flush(OriginLogLevel level);
OriginLogLevel origin_flush_level();

void origin_set_pattern(const char *pattern);
void origin_set_formatter(const FormatterSt *formatter);

void origin_flush();

void origin_force_log(const char *file, int line, const char *func, OriginLogLevel level,
                      const char *format, ...);

void origin_log(const char *file, int line, const char *func, OriginLogLevel level,
                const char *format, ...);
#pragma endregion

#pragma region Registry
bool register_logger(LoggerSt *logger);
void register_or_replace_logger(LoggerSt *logger);
void remove_logger(const char *name);
void remove_all();
LoggerSt *get_logger(const char *name);

void init_root_task_pool(uint32_t capacity, uint32_t threadCnt);
TaskPoolSt *root_task_pool();
#pragma endregion

#pragma region Logging manager
void initialize_logger(LoggerSt *logger,  bool autoRegister);
void set_level_all(OriginLogLevel level);
void flush_on_all(OriginLogLevel level);
void set_pattern_all(const char *pattern);
void set_formatter_all(FormatterSt *formatter);
void flush_all();
void shutdown();
#pragma endregion

#ifdef __cplusplus
}
#endif  // __cplusplus

#define ORIGIN_LOG_TRACE(fmt, ...) \
    origin_log(                    \
        __FILE__, __LINE__, __FUNCTION__, ORIGIN_LOG_LEVEL_TRACE, fmt __VA_OPT__(, ) __VA_ARGS__);
#define ORIGIN_LOG_DEBUG(fmt, ...) \
    origin_log(                    \
        __FILE__, __LINE__, __FUNCTION__, ORIGIN_LOG_LEVEL_DEBUG, fmt __VA_OPT__(, ) __VA_ARGS__);
#define ORIGIN_LOG_INFO(fmt, ...) \
    origin_log(                   \
        __FILE__, __LINE__, __FUNCTION__, ORIGIN_LOG_LEVEL_INFO, fmt __VA_OPT__(, ) __VA_ARGS__);
#define ORIGIN_LOG_WARN(fmt, ...) \
    origin_log(                   \
        __FILE__, __LINE__, __FUNCTION__, ORIGIN_LOG_LEVEL_WARN, fmt __VA_OPT__(, ) __VA_ARGS__);
#define ORIGIN_LOG_ERR(fmt, ...) \
    origin_log(                  \
        __FILE__, __LINE__, __FUNCTION__, ORIGIN_LOG_LEVEL_ERROR, fmt __VA_OPT__(, ) __VA_ARGS__);
#define ORIGIN_LOG_FATAL(fmt, ...) \
    origin_log(                    \
        __FILE__, __LINE__, __FUNCTION__, ORIGIN_LOG_LEVEL_FATAL, fmt __VA_OPT__(, ) __VA_ARGS__);

#endif  // ORIGIN_LOGGING_C_LOGGING_C_H
