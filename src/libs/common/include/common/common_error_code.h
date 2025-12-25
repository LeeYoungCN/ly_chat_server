#ifndef COMMON_CONSTANTS_ERROR_CODE_CONSTANTS_H
#define COMMON_CONSTANTS_ERROR_CODE_CONSTANTS_H

#include "common/types/error_code_types.h"

namespace common::error_code {
using namespace common::types::error_code;

#define COMMON_MODULE_ID 0
enum CommErrType {
    ERR_TYPE_COMM = 0,
    ERR_TYPE_DATE_TIME,
    ERR_TYPE_FILESYSTEM
};

enum class CommErrId {
    SUCCESS = 0,
    MALLOC_FAILED = 1,
    IDX_OUT_OF_BOUND,
    ENUM_INVALID,
    PARAM_NULL,
    PARAM_INVALID,
    BUFFER_OVERFLOW,
    SYSTEM_ERROR = 100,
    GENERIC_ERROR,
    UNKNOWN_ERROR
};

// 成功
#define ERR_COMM_SUCCESS         MAKE_ERROR(common::types::error_code::ERR_SEV_SUCCESS, COMMON_MODULE_ID, common::error_code::CommErrType::ERR_TYPE_COMM, common::error_code::CommErrId::SUCCESS)

#define MAKE_COMMON_ERROR(errId) MAKE_ERROR(common::types::error_code::ERR_SEV_ERROR, COMMON_MODULE_ID, common::error_code::CommErrType::ERR_TYPE_COMM, common::error_code::CommErrId::errId)
// 通用错误码
#define ERR_COMM_MALLOC_FAILED      MAKE_COMMON_ERROR(MALLOC_FAILED)
#define ERR_COMM_ARRAY_IDX_OOB      MAKE_COMMON_ERROR(IDX_OUT_OF_BOUND)
#define ERR_COMM_ENUM_INVALID       MAKE_COMMON_ERROR(ENUM_INVALID)
#define ERR_COMM_PARAM_NULL         MAKE_COMMON_ERROR(PARAM_NULL)
#define ERR_COMM_PARAM_INVALID      MAKE_COMMON_ERROR(PARAM_INVALID)
#define ERR_COMM_BUFFER_OVERFLOW    MAKE_COMMON_ERROR(BUFFER_OVERFLOW)
#define ERR_COMM_SYSTEM_ERROR       MAKE_COMMON_ERROR(SYSTEM_ERROR)
#define ERR_COMM_GENERIC_ERROR      MAKE_COMMON_ERROR(GENERIC_ERROR)
#define ERR_COMM_UNKNOWN_ERROR      MAKE_COMMON_ERROR(UNKNOWN_ERROR)

enum class DateTimeErrId {
    MONTH_INVALID = 1,
    WEEKDAY_INVALID,
    TIMEZONE_INVALID,
    TIMESTAMP_INVALID
};

#define MAKE_DATE_TIME_ERROR(errId) MAKE_ERROR(common::types::error_code::ERR_SEV_ERROR, COMMON_MODULE_ID, common::error_code::CommErrType::ERR_TYPE_DATE_TIME, common::error_code::DateTimeErrId::errId)
// datetime
#define ERR_COMM_MONTH_INVALID      MAKE_DATE_TIME_ERROR(MONTH_INVALID)
#define ERR_COMM_WEEKDAY_INVALID    MAKE_DATE_TIME_ERROR(WEEKDAY_INVALID)
#define ERR_COMM_TIMEZONE_INVALID   MAKE_DATE_TIME_ERROR(TIMEZONE_INVALID)
#define ERR_COMM_TIMESTAMP_INVALID  MAKE_DATE_TIME_ERROR(TIMESTAMP_INVALID)

enum class FileSysErrId {
    NOT_FOUND = 1,       ///< 路径不存在
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
};

#define MAKE_FILESYS_ERROR(errId) MAKE_ERROR(common::types::error_code::ERR_SEV_ERROR, COMMON_MODULE_ID, common::error_code::CommErrType::ERR_TYPE_FILESYSTEM, common::error_code::FileSysErrId::errId)

#define ERR_COMM_NOT_FOUND          MAKE_FILESYS_ERROR(NOT_FOUND)
#define ERR_COMM_PERMISSION_DENIED  MAKE_FILESYS_ERROR(PERMISSION_DENIED)
#define ERR_COMM_PATH_TOO_LONG      MAKE_FILESYS_ERROR(PATH_TOO_LONG)
#define ERR_COMM_ALREADY_EXISTS     MAKE_FILESYS_ERROR(ALREADY_EXISTS)
#define ERR_COMM_NOT_DIRECTORY      MAKE_FILESYS_ERROR(NOT_DIRECTORY)
#define ERR_COMM_NOT_FILE           MAKE_FILESYS_ERROR(NOT_FILE)
#define ERR_COMM_IO_ERROR           MAKE_FILESYS_ERROR(IO_ERROR)
#define ERR_COMM_DIR_NOT_EMPTY      MAKE_FILESYS_ERROR(DIR_NOT_EMPTY)
#define ERR_COMM_PATH_INVALID       MAKE_FILESYS_ERROR(PATH_INVALID)
#define ERR_COMM_SHARING_VIOLATION  MAKE_FILESYS_ERROR(SHARING_VIOLATION)
#define ERR_COMM_IS_A_DIRECTORY     MAKE_FILESYS_ERROR(IS_A_DIRECTORY)

inline const char *GetErrorMsg(ErrorCode errCode)
{
    switch (errCode) {
        case ERR_COMM_SUCCESS:
            return "success";
        case ERR_COMM_MALLOC_FAILED:
            return "malloc failed";
        case ERR_COMM_ARRAY_IDX_OOB:
            return "Input index array out of bound";
        case ERR_COMM_ENUM_INVALID:
            return "Enum invalid";
        case ERR_COMM_PARAM_NULL:
            return "Input param NULL";
        case ERR_COMM_PARAM_INVALID:
            return "Input param invalid";
        case ERR_COMM_BUFFER_OVERFLOW:
            return "Buffer overflow";
        case ERR_COMM_SYSTEM_ERROR:
            return "System error";
        case ERR_COMM_GENERIC_ERROR:
            return "Generic error";
        case ERR_COMM_UNKNOWN_ERROR:
        default:
            return "Unkown error";
    };
}
}  // namespace common::error_code

#endif  // COMMON_CONSTANTS_ERROR_CODE_CONSTANTS_H
