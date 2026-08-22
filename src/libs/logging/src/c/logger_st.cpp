#include <cstdarg>
#include <memory>
#include <stdexcept>

#include "c/common_c.h"
#include "internal/common.h"
#include "logging/c/logging_c.h"
#include "logging/loggers/async_logger.h"
#include "logging/loggers/sync_logger.h"

using namespace origin::logging;
using namespace origin::logging::c;

extern "C" {
LoggerSt *origin_create_sync_logger(const char *name, const SinkSt *const sinks[], uint32_t count)
{
    if (name == nullptr) {
        throw std::invalid_argument("name nullptr.");
    }

    if (sinks == nullptr || count == 0) {
        throw std::invalid_argument("sinks nullptr or count zero.");
    }

    return new struct LoggerSt(std::make_shared<SyncLogger>(name, sink_ptr_vector(sinks, count)));
}

LoggerSt *origin_create_async_logger(const char *name, const SinkSt *const sinks[], uint32_t count,
                                     const TaskPoolSt *taskPool)
{
    if (name == nullptr) {
        throw std::invalid_argument("name nullptr.");
    }

    if (sinks == nullptr || count == 0) {
        throw std::invalid_argument("sinks nullptr or count zero.");
    }

    if (taskPool == nullptr) {
        throw std::invalid_argument("taskPool nullptr.");
    }
    return new struct LoggerSt(
        std::make_shared<AsyncLogger>(name, sink_ptr_vector(sinks, count), taskPool->ptr));
}

void origin_destroy_logger(LoggerSt *logger)
{
    if (logger != nullptr) {
        if (logger->ptr != nullptr) {
            DEBUG_LOGGER_DBG("Release logger. Name: {}, UseCnt: {}.",
                             logger->ptr->name(),
                             logger->ptr.use_count());
            logger->ptr.reset();
        }
        delete logger;
    }
}

const char *origin_logger_name(LoggerSt *logger)
{
    RETURN_VALUE_IF_PTR_NULL(logger, "");
    return logger->ptr->name().data();
}

void origin_logger_set_level(LoggerSt *logger, OriginLogLevel level)
{
    RETURN_IF_PTR_NULL(logger);
    logger->ptr->set_level(c_to_cpp_log_level(level));
}

bool origin_logger_should_log(LoggerSt *logger, OriginLogLevel level)
{
    RETURN_VALUE_IF_PTR_NULL(logger, false);
    return logger->ptr->should_log(c_to_cpp_log_level(level));
}

OriginLogLevel origin_logger_level(LoggerSt *logger)
{
    RETURN_VALUE_IF_PTR_NULL(logger, ORIGIN_LOG_LEVEL_OFF);
    return cpp_to_c_log_level(logger->ptr->level());
}

void origin_logger_flush_on(LoggerSt *logger, OriginLogLevel level)
{
    RETURN_IF_PTR_NULL(logger);
    logger->ptr->flush_on(c_to_cpp_log_level(level));
}

bool origin_logger_should_flush(LoggerSt *logger, OriginLogLevel level)
{
    RETURN_VALUE_IF_PTR_NULL(logger, false);
    return logger->ptr->should_flush(c_to_cpp_log_level(level));
}

OriginLogLevel origin_logger_flush_level(LoggerSt *logger)
{
    RETURN_VALUE_IF_PTR_NULL(logger, ORIGIN_LOG_LEVEL_OFF);
    return cpp_to_c_log_level(logger->ptr->flush_level());
}

void origin_logger_set_pattern(LoggerSt *logger, const char *pattern)
{
    RETURN_IF_PTR_NULL(logger);
    RETURN_IF_PTR_NULL(pattern);

    logger->ptr->set_pattern(pattern);
}

void origin_logger_set_formatter(LoggerSt *logger, const FormatterSt *formatter)
{
    RETURN_IF_PTR_NULL(logger);
    RETURN_IF_PTR_NULL(formatter);
    RETURN_IF_PTR_NULL(formatter->ptr);

    logger->ptr->set_formatter(formatter->ptr->clone());
}

void origin_logger_flush(LoggerSt *logger)
{
    RETURN_IF_PTR_NULL(logger);
    logger->ptr->flush();
}

void origin_logger_log(LoggerSt *logger, const char *file, int line, const char *func,
                       OriginLogLevel level, const char *format, ...)
{
    RETURN_IF_PTR_NULL(logger);
    va_list args;
    va_start(args, format);
    origin_force_log_it(logger->ptr, file, line, func, c_to_cpp_log_level(level), format, args);
    va_end(args);
}
}
