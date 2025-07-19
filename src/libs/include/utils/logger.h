#ifndef LOGGER_H
#define LOGGER_H

#include <cstdint>
#include "common/base/singleton_base.h"

enum LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class Logger : public SingletonBase<Logger> {
    friend class SingletonBase<Logger>;
private:
    Logger() = default;
    ~Logger() override = default;
public:
    void init(LogLevel level, const char* logDir);
    void log(LogLevel level, const char *fileName, uint32_t line, const char *fmt, ...);
};

#endif // LOGGER_H
