# Logging 模块

## 一、模块概述

logging日志模块。

**Logger**: 日志器，收集日志
**Appender**: 日志输出器

## 二、 类型

```cpp
/**
 * 日志位置信息
 */
struct LogLocation {
    const char *file{nullptr};
    int32_t line{0};
    const char *func{nullptr};

    LogLocation() = default;
    LogLocation(const char *file, int32_t line, const char* func) : file(file), line(line), func(func) {}
}

```

## 二、 logging

```cpp
/**
 *  根据名称获取logger, 不存在则创建新的logger，如果name为空，返回默认日志
 */
Logger GetLogger(const std::string& name);

/**
 * 注册log
 */
Logger RegistLogger(const std::string& name, const std::shared_ptr<Logger>& logger);

void debug()
void info()
void warn()
void 

```

## 一、 Logger

## 二、 Formatter

## 三、 Appender

