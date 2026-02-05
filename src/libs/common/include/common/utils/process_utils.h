/**
 * @file process_utils.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2026-02-05
 *
 * @copyright Copyright (c) 2026
 *
 */
#pragma once

#ifndef COMMON_UTILS_PROCESS_H
#define COMMON_UTILS_PROCESS_H

#include <string>

#include "common/types/process_types.h"

namespace common::process {

/**
 * @brief 获取当前进程的可执行文件路径
 * @return 可执行文件的绝对路径
 * @note 不同平台实现方式不同：
 *       - Windows: 通过GetModuleFileName获取
 *       - Linux/macOS: 通过/proc/self/exe符号链接获取
 */
std::string GetProcessPath();

/**
 * @brief 获取当前进程所在目录
 *
 * @return 目录的绝对路径
 */
std::string GetProcessDirectory();

/**
 * @brief 获取当前进程名称
 *
 * @return std::string Process file name.
 */
std::string GetProcessFileName();

/**
 * @brief 获取当前进程ID
 * 
 * @return ProcessId 当前进程ID。
 */
ProcessId GetCurrProcessId();

}  // namespace common::process
#endif  // COMMON_UTILS_PROCESS_H
