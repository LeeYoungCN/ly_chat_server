# C++ 错误码与错误类别（`system_category` & `generic_category` 及扩展）参考文档

## 1. 概述

在 C++ 错误处理机制中，`std::error_category` 是抽象基类，用于定义错误码的分类体系。除了常见的 `std::system_category` 和 `std::generic_category`，C++ 标准及实践中还存在其他错误类别，用于处理特定场景（如 POSIX 扩展、Boost 库、自定义错误等）的错误码。本文将全面介绍各类错误类别及其关联的错误码定义。


## 2. 标准库内置错误类别

### 2.1 `std::generic_category` 详解

#### 2.1.1 定义与作用

`std::generic_category` 用于标识**C 标准库定义的通用错误码**，这些错误码在所有平台（Windows、Linux、macOS 等）上具有统一的含义，是跨平台错误处理的基础。

#### 2.1.2 错误码定义位置

- **核心头文件**：  
  - C 标准：`<errno.h>`（包含 `EINVAL`、`ENOMEM` 等宏定义）。  
  - C++ 标准：`<system_error>`（定义枚举 `std::errc`，是对 `<errno.h>` 错误码的类型安全封装）。  

- 包含方式：  

  ```cpp
  #include <cerrno>       // C++ 中包含 C 标准错误码宏
  #include <system_error> // 包含 std::errc 和 std::generic_category
  ```

#### 2.1.3 常见错误码示例

| 错误码宏（`<errno.h>`） | `std::errc` 枚举（`<system_error>`） | 含义                 |
|-------------------------|---------------------------------------|----------------------|
| `EINVAL`                | `std::errc::invalid_argument`         | 无效参数             |
| `ENOMEM`                | `std::errc::not_enough_memory`        | 内存不足             |
| `ENOENT`                | `std::errc::no_such_file_or_directory`| 文件或目录不存在     |


### 2.2 `std::system_category` 详解

#### 2.2.1 定义与作用

`std::system_category` 用于标识**操作系统特定的错误码**，这些错误码与底层系统调用或 API 紧密相关，不同平台的定义和含义可能不同。

#### 2.2.2 错误码定义位置（按平台区分）

- **类 Unix 系统（Linux/macOS）**：  
  - 基础系统错误码：`<errno.h>` 或 `<sys/errno.h>`（包含 POSIX 标准扩展错误码）。  
  - 内核级错误码：Linux 中可能位于 `<asm/errno.h>` 或 `<linux/errno.h>`。  

- **Windows 系统**：  
  - `<winerror.h>`（包含 Windows 原生 API 错误码，与 `GetLastError()` 返回值对应）。

#### 2.2.3 常见错误码示例（按平台区分）

- **类 Unix 系统**：`EAGAIN`（资源暂时不可用）、`EACCES`（权限被拒绝）。  
- **Windows 系统**：`ERROR_FILE_NOT_FOUND`（文件未找到）、`ERROR_ACCESS_DENIED`（访问被拒绝）。


### 2.3 `std::iostream_category`（C++11 新增）

#### 2.3.1 定义与作用

专门用于标识**I/O 流操作相关的错误**，例如流状态错误（如文件读取到末尾、格式错误等）。其错误码与 `std::ios_base::failure` 异常关联。

#### 2.3.2 错误码定义位置

- 头文件：`<ios>` 或 `<system_error>`。  
- 错误码值通常对应 `std::io_errc` 枚举（如 `std::io_errc::stream` 表示流错误）。

#### 2.3.3 使用示例

```cpp
#include <iostream>
#include <fstream>
#include <system_error>

int main() {
    std::ifstream file("nonexistent.txt");
    if (!file) {
        std::error_code ec = std::make_error_code(std::io_errc::stream);
        std::cout << "I/O 错误: " << ec.message() << "\n"; // 输出流相关错误描述
    }
    return 0;
}
```


## 3. 平台特定扩展错误类别

### 3.1 POSIX 扩展错误类别（类 Unix 系统）

#### 3.1.1 定义与作用

POSIX 标准在 C 标准基础上扩展了更多系统级错误码，这些错误码通常通过 `std::system_category` 标识，但部分实现（如 glibc）可能通过隐含类别处理。

#### 3.1.2 错误码定义位置

- 头文件：`<sys/errno.h>` 或 `<errno.h>`（包含 POSIX 扩展宏，如 `ENOSYS`（功能未实现）、`EOPNOTSUPP`（操作不支持））。


### 3.2 Windows 扩展错误类别

#### 3.2.1 定义与作用

Windows 系统除了基础 API 错误码，还包含组件特定错误（如 DirectX、COM 等），这些错误码通常通过自定义类别标识，但可通过 `std::system_category` 兼容处理。

#### 3.2.2 错误码定义位置

- 组件特定头文件：如 `d3d11.h`（DirectX 错误）、`winbase.h`（同步对象错误）。  
- 示例：`D3D11_ERROR_FILE_NOT_FOUND`（DirectX 文件未找到）、`RPC_S_SERVER_UNAVAILABLE`（COM 远程调用错误）。


## 4. 第三方库与自定义错误类别

### 4.1 Boost 库错误类别

#### 4.1.1 定义与作用

Boost 库（如 Boost.System）扩展了 C++ 错误处理机制，提供了 `boost::system::system_category` 和 `boost::system::generic_category`，与标准库兼容，同时新增了库特定错误（如 `boost::asio` 网络错误）。

#### 4.1.2 错误码定义位置

- 头文件：`<boost/system/error_code.hpp>`  
- 示例：`boost::asio::error::connection_refused`（网络连接被拒绝）。


### 4.2 自定义错误类别

#### 4.2.1 定义与作用

开发者可通过继承 `std::error_category` 实现自定义错误类别，用于标识应用程序特定的错误（如业务逻辑错误）。

#### 4.2.2 实现示例

```cpp
#include <system_error>
#include <string>

class custom_category : public std::error_category {
public:
    const char* name() const noexcept override { return "custom"; }
    std::string message(int ev) const override {
        switch (ev) {
            case 1: return "invalid user input";
            case 2: return "database connection failed";
            default: return "unknown error";
        }
    }
};

const custom_category custom_cat; // 全局实例

// 使用
std::error_code ec(1, custom_cat);
```


## 5. 各类别对比与最佳实践

### 5.1 关键区别总结

| 错误类别                | 错误码来源                  | 跨平台性       | 典型使用场景                  |
|-------------------------|-----------------------------|----------------|-------------------------------|
| `generic_category`      | C 标准库通用错误            | 完全兼容       | 基础逻辑错误（参数无效等）    |
| `system_category`       | 操作系统特定错误            | 平台相关       | 系统调用/API 错误             |
| `iostream_category`     | I/O 流操作错误              | 标准兼容       | 文件流、字符串流错误          |
| POSIX 扩展类别          | POSIX 系统调用扩展错误      | 类 Unix 兼容   | 进程管理、信号处理等          |
| Windows 扩展类别        | Windows 组件特定错误        | Windows 独有   | DirectX、COM 等组件错误       |
| Boost 错误类别          | Boost 库操作错误            | 跨平台（依赖库）| 网络、线程等库操作错误        |
| 自定义错误类别          | 应用程序业务逻辑            | 完全可控       | 业务规则违反、状态错误等      |


### 5.2 最佳实践

1. **跨平台优先**：通用逻辑错误优先使用 `generic_category` 和 `std::errc`。  
2. **系统操作适配**：调用系统 API 时使用 `system_category` 处理平台特定错误。  
3. **I/O 流专用**：流操作错误优先使用 `iostream_category`。  
4. **扩展场景**：使用第三方库时遵循其错误类别规范（如 Boost.Asio）。  
5. **自定义需求**：业务错误通过自定义类别封装，提高代码可读性。  


通过合理选择错误类别，可使错误处理逻辑更清晰、可移植性更强，同时便于调试和维护。
