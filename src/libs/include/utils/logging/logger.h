/**
 * @file logger.h
 * @brief 高性能异步日志模块头文件
 * @author Your Name
 * @date 2023-07-21
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <condition_variable>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <queue>
#include <thread>

#include "common/base/singleton_base.h"
#include "common/time/time.h"

/**
 * @enum LogLevel
 * @brief 日志级别枚举
 */
enum class LogLevel {
    DEBUG,    //  调试信息
    INFO,     // 一般信息
    WARNING,  // 警告信息
    ERROR,    //  错误信息
    FATAL,    //  致命错误
};

/**
 * @enum LogRollingPolicy
 * @brief 日志滚动策略
 */
enum class LogRollingPolicy {
    NONE,          ///< 不滚动
    SIZE,          ///< 按大小滚动
    TIME,          ///< 按时间滚动
    SIZE_AND_TIME  ///< 按大小和时间滚动
};

/**
 * @enum LogTimeSplitPolicy
 * @brief 日志时间分割策略
 */
enum class LogTimeSplitPolicy {
    NONE,   ///< 不按时间分割
    DAILY,  ///< 按天分割
    HOURLY  ///< 按小时分割
};

/**
 * @enum LogTarget
 * @brief 日志输出目标
 */
enum class LogTarget {
    FILE,     ///< 仅输出到文件
    CONSOLE,  ///< 仅输出到控制台
    BOTH      ///< 同时输出到文件和控制台
};

struct LoggerInitParams {
    const std::string baseFileName;
    LogLevel logLevel = LogLevel::INFO;
    uint32_t maxBufferSize = 1024;
    uint32_t flushIntervalSec = 1;
    uint32_t maxFileSize = 1024 * 1024;
    LogRollingPolicy rollingPolicy = LogRollingPolicy::SIZE_AND_TIME;
    LogTimeSplitPolicy timeSplitPolicy = LogTimeSplitPolicy::DAILY;
    LogTarget logTarget = LogTarget::FILE;
};

/**
 * @class Logger
 * @brief 高性能异步日志记录器，采用单例模式实现
 *
 * 该日志器支持异步写入、多级别日志过滤、日志文件滚动和时间分割等功能，
 * 能够在高并发场景下保持良好的性能表现。
 */
class Logger : public SingletonBase<Logger> {
    friend class SingletonBase<Logger>;

private:
    Logger() = default;
    ~Logger() override;

public:
    /**
     * @brief 初始化日志系统
     *
     * @param baseFileName 基础日志文件名
     * @param maxBufferSize 日志缓冲区大小（条目数）
     * @param flushIntervalSec 刷新间隔（秒）
     * @param maxFileSize 最大文件大小（字节），仅在按大小滚动时有效
     * @param rollingPolicy 日志滚动策略
     * @param timeSplitPolicy 时间分割策略
     */
    void init(const LoggerInitParams &params);
    /**
     * @brief 设置日志级别
     * @param level 日志级别
     */
    void setLogLevel(LogLevel level);
    void setLogTarget(LogTarget target);
    /**
     * @brief 记录日志,如果日志模块未运行, 则直接打印在屏幕上。
     *
     * @param level 日志等级
     * @param fileName 日志所在的源文件名称
     * @param line 日志行号
     * @param fmt 日志内容格式
     * @param ... 要打印的内容
     */
    void log(LogLevel level, const char *fileName, uint32_t line, const char *fmt, ...);

private:
    std::string getLogLevelStr(LogLevel level);
    void openNewLogFile();
    void closeLogFile();
    void backupLogFile();
    void checkAndRollLogFile();
    void logWorkerThreader();
    void printLog(LogLevel level, const std::string& logStr);
    void flushLog();
    void shutdown();

private:
    std::string m_baseLogFile;
    std::string m_logFolder;
    std::ofstream m_logFileStream{};
    LogLevel m_logLevel{LogLevel::INFO};

    uint32_t m_maxBufferSize{1024};
    uint32_t m_flushIntervalSec{1};
    std::queue<std::string> m_logBuffer;

    uint32_t m_maxFileSize{1024 * 1024};
    LogRollingPolicy m_roolingPolicy{LogRollingPolicy::SIZE_AND_TIME};
    LogTimeSplitPolicy m_timeSplitPolicy{LogTimeSplitPolicy::DAILY};
    std::chrono::steady_clock::time_point m_lastFlushTime;
    proj::TimeSt m_lastSplitTime;

    std::atomic<bool> m_isRunning{false};
    std::thread m_workThread;
    std::mutex m_bufferMutex;
    std::condition_variable m_flushCondition;
    LogTarget m_logTargt = LogTarget::FILE;
};

#define LOG(level, fmt, ...) INST(Logger).log(level, __FILE__, __LINE__, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_DEBUG(fmt, ...) LOG(LogLevel::DEBUG, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_INFO(fmt, ...) LOG(LogLevel::INFO, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_WARNING(fmt, ...) LOG(LogLevel::WARNING, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_ERROR(fmt, ...) LOG(LogLevel::ERROR, fmt __VA_OPT__(, ) __VA_ARGS__)
#define LOG_EVENT(fmt, ...) LOG(LogLevel::EVENT, fmt __VA_OPT__(, ) __VA_ARGS__)
#endif  // LOGGER_H
