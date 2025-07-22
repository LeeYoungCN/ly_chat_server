#include "utils/logging/logger.h"

#include <chrono>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <system_error>
#include <thread>

#include "common/file_system/file_system.h"
#include "common/time/time.h"

namespace fs = std::filesystem;

Logger::~Logger()
{
    shutdown();
}

void Logger::init(const LoggerInitParams &params)
{
    m_baseLogFile = params.baseFileName;
    m_logLevel = params.logLevel;
    m_maxBufferSize = params.maxBufferSize;
    m_flushIntervalSec = params.flushIntervalSec;
    m_maxFileSize = params.maxBufferSize;
    m_roolingPolicy = params.rollingPolicy;
    m_timeSplitPolicy = params.timeSplitPolicy;
    m_maxFileSize = params.maxFileSize;
    m_logTargt = params.logTarget;
    fs::path fsLogFolder = fs::path(m_baseLogFile).parent_path();
    m_logFolder = fsLogFolder.string();

    if (!fsLogFolder.empty()) {
        if (!proj::CreateDirectoryIfNotExist(m_logFolder)) {
            LOG_ERROR("Failed to create log directory: %s", m_logFolder.c_str());
            return;
        } else {
            LOG_INFO("Directory created successfully: %s", m_logFolder.c_str());
        }
    }

    openNewLogFile();

    m_lastFlushTime = std::chrono::steady_clock::now();
    m_isRunning = true;
    m_workThread = std::thread(&Logger::logWorkerThreader, this);
    LOG_INFO("Logger initialized: %s", m_baseLogFile.c_str());
}

void Logger::setLogLevel(LogLevel level)
{
    m_logLevel = level;
}

void Logger::setLogTarget(LogTarget target)
{
    m_logTargt = target;
}

void Logger::log(LogLevel level, const char *fileName, uint32_t line, const char *fmt, ...)
{
    if (!m_isRunning && m_logTargt != LogTarget::CONSOLE && m_logTargt != LogTarget::BOTH) {
        return;
    }

    if (level < m_logLevel) {
        return;
    }
    constexpr uint32_t BUFFER_LEN = 542;
    char buffer[BUFFER_LEN];
    va_list argList;
    va_start(argList, fmt);
    vsprintf(buffer, fmt, argList);
    va_end(argList);

    std::string logStr = "[" + proj::FormatCurrTimeStr("%Y-%m-%d %H:%M:%S") + "]" + "[" + getLogLevelStr(level) + "]" +
                         "[" + basename(fileName) + ":" + std::to_string(line) + "] " + buffer;

    printLog(level, logStr);

    if (!m_isRunning) {
        return;
    }

    {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        m_logBuffer.emplace(logStr);
        bool needFlush = false;
        auto now = std::chrono::steady_clock::now();
        if (!needFlush && m_logBuffer.size() >= m_maxBufferSize) {
            needFlush = true;
        }
        if (!needFlush && now - m_lastFlushTime >= std::chrono::seconds(m_flushIntervalSec)) {
            needFlush = true;
        }
        if (needFlush) {
            m_flushCondition.notify_one();
        }
    }
}

std::string Logger::getLogLevelStr(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}

void Logger::openNewLogFile()
{
    backupLogFile();
    m_logFileStream.open(m_baseLogFile, std::ios::out | std::ios::trunc);
    m_lastSplitTime = proj::GetCurrentTime();
    if (!m_logFileStream.is_open()) {
        LOG_ERROR("Failed to open log file: %s", m_baseLogFile.c_str());
    } else {
        LOG_INFO("New log file opened: %s", m_baseLogFile.c_str());
    }
}

void Logger::backupLogFile()
{
    fs::path logFullPath = fs::path(m_baseLogFile);
    std::string baseFileName = logFullPath.stem().string();
    std::string extention = logFullPath.extension().string();

    if (proj::IsRegularFile(m_baseLogFile) &&
        (m_roolingPolicy == LogRollingPolicy::SIZE_AND_TIME || m_roolingPolicy == LogRollingPolicy::TIME)) {
        std::string timeStamp = proj::FormatCurrTimeStr("%Y%m%d%H%M%S");
        std::string backupLogFile = m_logFolder + "/" + baseFileName + "." + timeStamp + extention;
        closeLogFile();
        fs::rename(m_baseLogFile, backupLogFile);
        LOG_INFO("Backup log file: %s", backupLogFile.c_str());
    }
}

void Logger::logWorkerThreader()
{
    while (m_isRunning) {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        m_flushCondition.wait_for(lock, std::chrono::seconds(m_flushIntervalSec), [this]() -> bool {
            return !m_isRunning || !m_logBuffer.empty();
        });

        flushLog();
        checkAndRollLogFile();
    }
}

void Logger::shutdown()
{
    if (!m_isRunning) {
        return;
    }
    LOG_INFO("Logger shut down!.");
    {
        std::unique_lock<std::mutex> lock(m_bufferMutex);
        m_flushCondition.notify_one();
        m_isRunning = false;
    }
    if (m_workThread.joinable()) {
        m_workThread.join();
    }
    flushLog();
    closeLogFile();
}

void Logger::printLog(LogLevel level, const std::string &logStr)
{
    if (m_logTargt == LogTarget::BOTH || m_logTargt == LogTarget::CONSOLE) {
        if (level == LogLevel::ERROR || level == LogLevel::FATAL) {
            std::cerr << logStr << std::endl;
        } else {
            std::cout << logStr << std::endl;
        }
    }
}

void Logger::flushLog()
{
    if (m_logBuffer.empty()) {
        return;
    }
    while (!m_logBuffer.empty()) {
        if (m_logFileStream.is_open() && (m_logTargt == LogTarget::BOTH || m_logTargt == LogTarget::FILE)) {
            m_logFileStream << m_logBuffer.front() << std::endl;
        }
        m_logBuffer.pop();
    }
    if (m_logFileStream.is_open()) {
        m_logFileStream.flush();
    }
    m_lastFlushTime = std::chrono::steady_clock::now();
}

void Logger::checkAndRollLogFile()
{
    if (!m_logFileStream.is_open()) {
        return;
    }
    bool needRoll = false;
    if (!needRoll &&
        (m_roolingPolicy == LogRollingPolicy::SIZE_AND_TIME || m_roolingPolicy == LogRollingPolicy::SIZE)) {
        needRoll = (fs::file_size(m_baseLogFile) >= m_maxFileSize);
    }
    if (!needRoll && m_timeSplitPolicy != LogTimeSplitPolicy::NONE &&
        (m_roolingPolicy == LogRollingPolicy::SIZE_AND_TIME || m_roolingPolicy == LogRollingPolicy::TIME)) {
        auto now = proj::GetCurrentTime();
        switch (m_timeSplitPolicy) {
            case LogTimeSplitPolicy::DAILY:
                needRoll = (now.day != m_lastSplitTime.day);
                break;
            case LogTimeSplitPolicy::HOURLY:
                needRoll = (now.hour != m_lastSplitTime.hour);
                break;
            default:
                break;
        }
    }
    if (needRoll) {
        openNewLogFile();
    }
}

void Logger::closeLogFile()
{
    if (m_logFileStream.is_open()) {
        m_logFileStream.close();
    }
    LOG_ERROR("Close log file: %s\n", m_baseLogFile.c_str());
}
