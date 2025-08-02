# 通用文件系统工具模块说明文档

## 一、模块概述

**common::utils::filesystem** 模块是跨平台文件系统操作工具集，采用分层命名空间设计：

- **类型定义**：`common::types::filesystem`（包含枚举、结构体等类型）
- **常量定义**：`common::constants::filesystem`（包含路径长度、分隔符等常量）
- **工具接口**：`common::utils::filesystem`（提供文件系统操作实现）

模块适配 Windows、Linux、macOS 平台，接口设计遵循：

- 枚举类型采用大驼峰命名
- 常量和枚举值采用全大写+下划线命名
- 接口和结构体成员采用大驼峰命名
- 无任何匈牙利命名前缀

## 二、类型定义（`common::types::filesystem`）

### 2.1 基础类型别名

```cpp
namespace common {
namespace types {
namespace filesystem {
    using PathString = std::string;          // 路径字符串类型
    using PathList = std::vector<PathString>; // 路径列表类型
    using FileSize = uint64_t;               // 文件大小类型（字节）
    using ByteVector = std::vector<uint8_t>; // 二进制数据容器
} // namespace filesystem
} // namespace types
} // namespace common
```

### 2.2 枚举类型（大驼峰命名）

```cpp
namespace common {
namespace types {
namespace filesystem {
    // 文件/目录条目类型
    enum class EntryType {
        NONEXISTENT = 0,   // 路径不存在
        FILE,              // 普通文件
        DIRECTORY,         // 目录
        SYMBOL_LINK,       // 符号链接
        CHARACTER_DEVICE,  // 字符设备
        BLOCK_DEVICE,      // 块设备
        FIFO,              // 管道(FIFO)
        SOCKET,            // 套接字
        UNKNOWN            // 未知类型
    };

    // 文件权限枚举
    enum class Permission {
        None = 0,      // 无权限
        Read = 1 << 0, // 读权限
        Write = 1 << 1,// 写权限
        Execute = 1 << 2, // 执行权限
        ReadWrite = Read | Write,
        All = Read | Write | Execute
    };

    // 文件属性枚举
    enum class FileAttribute {
        None = 0,
        Hidden = 1 << 0,   // 隐藏属性
        Readonly = 1 << 1, // 只读属性
        System = 1 << 2    // 系统文件（仅Windows）
    };
} // namespace filesystem
} // namespace types
} // namespace common
```

### 2.3 结构体类型

```cpp
namespace common {
namespace types {
namespace filesystem {
    // 文件信息结构体
    struct FileInfo {
        PathString path;                                 ///< 文件的完整路径
        FileSize size;                                   ///< 文件大小（字节）
        EntryType type;                                  ///< 条目类型（文件/目录等）
        common::types::date_time::Timestamp modifyTime;  ///< 最后修改时间戳）
    };
} // namespace filesystem
} // namespace types
} // namespace common
```

## 三、常量与枚举值

### 3.1 路径与尺寸常量（`common::constants::filesystem`）

```cpp
namespace common {
namespace constants {
namespace filesystem {
    // 路径长度限制（跨平台自动适配）
#ifdef _WIN32
    constexpr size_t MAX_PATH_STD = 260;       // Windows标准路径最大长度
    constexpr size_t MAX_PATH_LONG = 32767;    // Windows长路径最大长度
#else
    constexpr size_t MAX_PATH_STD = 4096;      // Unix系统标准路径最大长度
    constexpr size_t MAX_PATH_LONG = 65536;    // Unix系统扩展路径长度
#endif

    // 文件名最大长度（不含路径）
#ifdef _WIN32
    constexpr size_t MAX_FILENAME = 256;       // Windows文件名长度限制
#else
    constexpr size_t MAX_FILENAME = 255;       // Unix文件名长度限制
#endif

    // 路径分隔符（平台相关）
#ifdef _WIN32
    constexpr const char* PATH_SEP = "\\";     // Windows路径分隔符
    constexpr const char* ALT_PATH_SEP = "/";  // Windows兼容分隔符
#else
    constexpr const char* PATH_SEP = "/";      // Unix系统路径分隔符
    constexpr const char* ALT_PATH_SEP = "";   // 无替代分隔符
#endif
} // namespace filesystem
} // namespace constants
} // namespace common
```

### 3.2 错误码枚举

```cpp
namespace common {
namespace constants {
namespace filesystem {
    // 错误码枚举
    enum class ErrorCode {
        SUCCESS = 0,         ///< 操作成功
        NOT_FOUND,           ///< 路径不存在
        PERMISSION_DENIED,   ///< 权限不足
        PATH_TOO_LONG,       ///< 路径长度超过系统限制
        ALREADY_EXISTS,      ///< 目标路径已存在
        NOT_DIRECTORY,       ///< 路径指向的不是目录
        NOT_FILE,            ///< 路径指向的不是文件
        IO_ERROR,            ///< I/O操作错误
        DIR_NOT_EMPTY,       ///< 文件夹非空
        PATH_INVALID,        ///< 路径不合法
        SHARING_VIOLATION,   ///< 文件共享冲突
        IS_A_DIRECTORY,      ///< 目标是文件夹
        SYSTEM_ERROR = 100,  ///< 系统级错误
        GENERIC_ERROR,       ///< 标准库通用错误
        UNKNOWN_ERROR        ///< 未知错误
    };
} // namespace filesystem
} // namespace constants
} // namespace common
```

## 四、工具接口（`common::utils::filesystem`）

### 4.1 路径处理接口（大驼峰命名）

| 接口名              | 功能描述                          | 参数说明                                  | 返回值类型                                   |
|---------------------|-----------------------------------|-------------------------------------------|----------------------------------------------|
| `JoinPaths`         | 拼接多个路径片段                  | `const PathList& parts`                   | `PathString`                                 |
| `NormalizePath`     | 规范化路径（去除./和../）         | `const PathString& path`                  | `PathString`                                 |
| `ToAbsolutePath`    | 转换为绝对路径                    | `const PathString& relPath`               | `PathString`                                 |
| `GetDirName`        | 获取父目录路径                    | `const PathString& path`                  | `PathString`                                 |
| `GetBaseName`       | 获取带扩展名的文件名              | `const PathString& path`                  | `PathString`                                 |
| `GetFileName`       | 获取不带扩展名的文件名            | `const PathString& path`                  | `PathString`                                 |
| `GetExtension`      | 获取文件扩展名（含.）             | `const PathString& path`                  | `PathString`                                 |
| `IsAbsolutePath`    | 判断是否为绝对路径                | `const PathString& path`                  | `bool`                                       |
| `IsPathTooLong`     | 判断路径是否超过长度限制          | `const PathString& path`                  | `bool`                                       |

**示例**：

```cpp
using namespace common;

// 拼接路径
types::filesystem::PathList parts = {"data", "config", "app.json"};
auto configPath = utils::filesystem::JoinPaths(parts);
// Windows: "data\config\app.json"
// Unix: "data/config/app.json"
```

### 4.2 文件操作接口

| 接口名              | 功能描述                          | 参数说明                                  | 返回值类型                                   |
|---------------------|-----------------------------------|-------------------------------------------|----------------------------------------------|
| `FileExists`        | 判断文件是否存在                  | `const PathString& path`                  | `bool`                                       |
| `CreateFile`        | 创建空文件                        | `const PathString& path`                  | `bool`                                       |
| `DeleteFile`        | 删除文件                          | `const PathString& path`                  | `bool`                                       |
| `CopyFile`          | 复制文件                          | `const PathString& src, const PathString& dest, bool overwrite` | `bool` |
| `RenameFile`        | 移动或重命名文件                   | `const PathString& src, const PathString& dest, bool overwrite` | `bool` |
| `ReadTextFile`      | 读取文本文件内容                  | `const PathString& path`                  | `PathString`                                 |
| `ReadBinaryFile`    | 读取二进制文件内容                | `const PathString& path`                  | `ByteVector`                                 |
| `WriteTextFile`     | 写入文本内容到文件                | `const PathString& path, const PathString& content, bool overwrite` | `bool` |
| `WriteBinaryFile`   | 写入二进制内容到文件              | `const PathString& path, const ByteVector& data, bool append` | `bool` |
| `GetFileSize`       | 获取文件大小（字节）              | `const PathString& path`                  | `FileSize`                                   |
| `GetFileInfo`       | 获取文件详细信息                  | `const PathString& path`                  | `FileInfo`                                   |
| `SetFilePermissions`| 设置文件权限                      | `const PathString& path, Permission perm` | `bool`                                       |

### 4.3 目录操作接口

| 接口名              | 功能描述                          | 参数说明                                  | 返回值类型                                   |
|---------------------|-----------------------------------|-------------------------------------------|----------------------------------------------|
| `DirExists`         | 判断目录是否存在                  | `const PathString& path`                  | `bool`                                       |
| `CreateDir`         | 创建目录（支持递归创建）          | `const PathString& path, bool recursive`  | `bool`                                       |
| `DeleteDir`         | 删除目录（支持递归删除）          | `const PathString& path, bool recursive`  | `bool`                                       |
| `ListDir`           | 列出目录中的所有条目              | `const PathString& path`                  | `PathList`                                   |
| `ListDirDetailed`   | 列出目录中所有条目的详细信息      | `const PathString& path`                  | `std::vector<FileInfo>`                      |
| `GetDirSize`        | 计算目录总大小（含子目录）        | `const PathString& path`                  | `FileSize`                                   |

### 4.4 错误处理接口

| 接口名              | 功能描述                          | 参数说明                                  | 返回值类型                                   |
|---------------------|-----------------------------------|-------------------------------------------|----------------------------------------------|
| `GetLastError`      | 获取最后一次操作的错误码          | 无                                        | `constants::filesystem::ErrorCode`           |
| `GetErrorString`    | 将错误码转换为描述字符串          | `ErrorCode code`                          | `std::string`                                |

## 五、完整使用示例

```cpp
#include "common/types/filesystem.h"
#include "common/constants/filesystem.h"
#include "common/utils/filesystem.h"
#include <iostream>

int main() {
    using namespace common;
    
    // 定义路径
    types::filesystem::PathString baseDir = "app_data";
    types::filesystem::PathString logDir = utils::filesystem::JoinPaths({baseDir, "logs"});
    types::filesystem::PathString logFile = utils::filesystem::JoinPaths({logDir, "app.log"});

    // 创建目录
    if (!utils::filesystem::DirExists(logDir)) {
        if (utils::filesystem::CreateDir(logDir, true)) {
            std::cout << "目录创建成功: " << logDir << std::endl;
        } else {
            auto error = utils::filesystem::GetLastError();
            std::cerr << "目录创建失败: " << utils::filesystem::GetErrorString(error) << std::endl;
            return 1;
        }
    }

    // 写入日志文件
    if (utils::filesystem::WriteTextFile(logFile, "Application started successfully", true)) {
        std::cout << "日志写入成功" << std::endl;
        
        // 获取文件信息
        auto fileInfo = utils::filesystem::GetFileInfo(logFile);
        std::cout << "文件大小: " << fileInfo.Size << " bytes" << std::endl;
        std::cout << "最后修改时间: " << ctime(&fileInfo.ModifyTime);
    }

    return 0;
}
```

## 六、命名规范说明

1. **类型命名**：
   - 枚举类型：大驼峰（如 `EntryType`、`Permission`）
   - 结构体类型：大驼峰（如 `FileInfo`）
   - 类型别名：大驼峰（如 `PathString`、`FileSize`）

2. **常量与枚举值命名**：
   - 常量：全大写+下划线（如 `PATH_MAX_STD`、`PATH_SEP`）
   - 枚举值：全大写+下划线（如 `SUCCESS`、`NOT_FOUND`）

3. **接口与成员命名**：
   - 函数接口：大驼峰（如 `JoinPaths`、`CreateDir`）
   - 结构体成员：大驼峰（如 `FileInfo::Path`、`FileInfo::Size`）

4. **命名空间结构**：
   - `common::types::filesystem`：仅包含类型定义
   - `common::constants::filesystem`：仅包含常量和错误码
   - `common::utils::filesystem`：仅包含操作接口

该命名规范保持了代码的清晰性和一致性，避免了冗余前缀，同时通过命名空间明确区分了不同类型的组件，适合大型项目开发使用。
