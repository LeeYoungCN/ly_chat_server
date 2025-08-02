/**
 * @file file_system.h
 * @author LiYang
 * @brief 文件系统封装
 * @version 0.1
 * @date 2025-07-22
 *
 * @copyright Copyright (c) 2025
 *
 */
#ifndef COMMON_UTILS_FILESYSTEM_UTILS_H
#define COMMON_UTILS_FILESYSTEM_UTILS_H

#include "common/constants/filesystem_constants.h"
#include "common/types/filesystem_types.h"

namespace common::utils::filesystem {

/**
 * @brief 判断指定路径的文件系统条目类型
 *
 * 该函数使用C++17标准文件系统库，跨平台判断给定路径对应的
 * 文件系统条目类型（如文件、目录、符号链接等），或路径是否不存在。
 * 相比直接使用平台API，该实现具有良好的可移植性。
 *
 * @param path 待判断的路径字符串（支持相对路径和绝对路径）
 * @return EntryType 条目类型枚举值：
 *         - EntryType::Nonexistent：路径不存在
 *         - EntryType::File：普通文件
 *         - EntryType::Directory：目录
 *         - EntryType::SymbolicLink：符号链接（软链接）
 *         - EntryType::CharacterDevice：字符设备文件
 *         - EntryType::BlockDevice：块设备文件
 *         - EntryType::FIFO：FIFO管道文件
 *         - EntryType::Socket：套接字文件
 *         - EntryType::Unknown：无法识别的类型或获取信息失败（如权限不足）
 *
 * @note 函数内部处理了符号链接的特殊情况：返回链接本身的类型，
 *       而非链接指向的目标类型。如需获取目标类型，需先解析链接。
 */
common::types::filesystem::EntryType GetEntryType(const types::filesystem::PathString& path);

const char* GetEntryTypeString(common::types::filesystem::EntryType type);
/**
 * @brief 获取当前进程的可执行文件路径
 * @return 可执行文件的绝对路径
 * @note 不同平台实现方式不同：
 *       - Windows: 通过GetModuleFileName获取
 *       - Linux/macOS: 通过/proc/self/exe符号链接获取
 */
types::filesystem::PathString GetProcessPath();

/**
 * @brief 获取当前进程所在目录
 *
 * @return 目录的绝对路径
 */
types::filesystem::PathString GetProcessDirName();

/**
 * @brief 获取当前工作路径。
 *
 * @return 工作路径
 */
types::filesystem::PathString GetCurrentWorkingDirectory();

/**
 * @brief 拼接多个路径片段为完整路径
 * @param parts 路径片段列表
 * @return 拼接后的完整路径字符串
 * @note 自动处理不同平台的路径分隔符
 */
types::filesystem::PathString JoinPaths(const types::filesystem::PathList& parts);

/**
 * @brief 规范化路径，去除其中的./和../等冗余部分
 * @param path 原始路径
 * @return 规范化后的路径
 */
types::filesystem::PathString NormalizePath(const types::filesystem::PathString& path);

/**
 * @brief 将相对路径转换为绝对路径
 * @param relPath 相对路径
 * @param baseDir 基准目录（默认为当前工作目录）
 * @return 转换后的绝对路径
 */
types::filesystem::PathString ToAbsolutePath(const types::filesystem::PathString& relPath,
                                             const types::filesystem::PathString& baseDir = "");

/**
 * @brief 获取路径中的目录部分（父目录路径）
 * @param path 完整路径
 * @return 父目录路径
 */
types::filesystem::PathString GetDirName(const types::filesystem::PathString& path);

/**
 * @brief 获取路径中的文件名（含扩展名）
 * @param path 完整路径
 * @return 带扩展名的文件名
 */
types::filesystem::PathString GetBaseName(const types::filesystem::PathString& path);

/**
 * @brief 获取路径中的文件名（不含扩展名）
 * @param path 完整路径
 * @return 不带扩展名的文件名
 */
types::filesystem::PathString GetFileName(const types::filesystem::PathString& path);

/**
 * @brief 获取文件的扩展名（含.）
 * @param path 完整路径或文件名
 * @return 文件扩展名（如".txt"），无扩展名则返回空字符串
 */
types::filesystem::PathString GetExtension(const types::filesystem::PathString& path);

/**
 * @brief 判断路径是否为绝对路径
 * @param path 待判断的路径
 * @return 是绝对路径返回true，否则返回false
 */
bool IsAbsolutePath(const types::filesystem::PathString& path);

/**
 * @brief 判断路径长度是否超过系统限制
 * @param path 待检查的路径
 * @return 超过限制返回true，否则返回false
 */
bool IsPathTooLong(const types::filesystem::PathString& path);

/**
 * @brief 判断文件是否存在且为普通文件
 * @param path 文件路径
 * @return 存在且为文件返回true，否则返回false
 */
bool FileExists(const types::filesystem::PathString& path);

/**
 * @brief 创建空文件
 * @param path 要创建的文件路径
 * @return 创建成功返回true，否则返回false
 * @note 若文件已存在，将被截断为0字节
 */
bool CreateFileUtils(const common::types::filesystem::PathString& path);

/**
 * @brief 删除文件
 * @param path 要删除的文件路径
 * @return 删除成功返回true，否则返回false
 */
bool DeleteFileUtils(const common::types::filesystem::PathString& path);

/**
 * @brief 复制文件
 * @param src 源文件路径
 * @param dest 目标文件路径
 * @param overwrite 若目标已存在，是否覆盖（默认为false）
 * @return 复制成功返回true，否则返回false
 */
bool CopyFileUtils(const types::filesystem::PathString& src, const types::filesystem::PathString& dest,
                   bool overwrite = false);

/**
 * @brief 读取文本文件内容
 * @param path 文本文件路径
 * @return 文件内容字符串，读取失败返回空字符串
 */
std::string ReadTextFileUtils(const types::filesystem::PathString& path);

/**
 * @brief 读取二进制文件内容
 * @param path 二进制文件路径
 * @return 二进制数据容器，读取失败返回空容器
 */
types::filesystem::ByteVector ReadBinaryFile(const types::filesystem::PathString& path);

/**
 * @brief 写入文本内容到文件
 * @param path 目标文件路径
 * @param content 要写入的文本内容
 * @param append 是否以追加模式写入（默认为false，覆盖模式）
 * @return 写入成功返回true，否则返回false
 */
bool WriteTextFileUtils(const types::filesystem::PathString& path, const types::filesystem::PathString& content,
                        bool append = false);

/**
 * @brief 获取文件大小（字节数）
 * @param path 文件路径
 * @return 文件大小（字节），获取失败返回0
 */
types::filesystem::FileSize GetFileSizeUtils(const types::filesystem::PathString& path);

/**
 * @brief 获取文件的详细信息
 * @param path 文件路径
 * @return 包含文件信息的FileInfo结构体，获取失败返回默认初始化的结构体
 */
types::filesystem::FileInfo GetFileInfo(const types::filesystem::PathString& path);

/**
 * @brief 判断目录是否存在且为目录
 * @param path 目录路径
 * @return 存在且为目录返回true，否则返回false
 */
bool DirExists(const types::filesystem::PathString& path);

/**
 * @brief 创建目录
 * @param path 要创建的目录路径
 * @param recursive 是否递归创建父目录（默认为false）
 * @return 创建成功返回true，否则返回false
 */
bool CreateDir(const types::filesystem::PathString& path, bool recursive = true);

/**
 * @brief 删除目录
 * @param path 要删除的目录路径
 * @param recursive 是否递归删除非空目录（默认为false）
 * @return 删除成功返回true，否则返回false
 */
bool DeleteDir(const types::filesystem::PathString& path, bool recursive = true);

/**
 * @brief 获取最后一次操作的错误码
 * @return 错误码枚举值
 */
constants::filesystem::ErrorCode GetLastError();

/**
 * @brief 将最近的错误码转换为人类可读的描述字符串
 * @param code 错误码
 * @return 错误描述字符串
 */
const char* GetLastErrorString();

}  // namespace common::utils::filesystem
#endif  // COMMON_UTILS_FILESYSTEM_UTILS_H
