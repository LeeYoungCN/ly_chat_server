#ifndef COMMON_CONSTANTS_ERROR_CODE_CONSTANTS_H
#define COMMON_CONSTANTS_ERROR_CODE_CONSTANTS_H

#include "common/types/error_code_types.h"

#define COMMON_MODULE_ID 1
typedef enum {
    COMM_ERR_TYPE_COMM = 1,
    COMM_ERR_TYPE_DATE_TIME,
    COMM_ERR_TYPE_FILESYSTEM,
    COMM_ERR_TYPE_CONTAINER
} CommErrType;

typedef enum {
    COMM_ERR_ID_SUCCESS = 0,
    COMM_ERR_ID_MALLOC_FAILED = 1,
    COMM_ERR_ID_IDX_OUT_OF_BOUND,
    COMM_ERR_ID_ENUM_INVALID,
    COMM_ERR_ID_PARAM_NULL,
    COMM_ERR_ID_PARAM_INVALID,
    COMM_ERR_ID_BUFFER_OVERFLOW,
    COMM_ERR_ID_SYSTEM_ERROR = 100,
    COMM_ERR_ID_GENERIC_ERROR,
    COMM_ERR_ID_UNKNOWN_ERROR
} CommErrId;

// 成功
#define ERR_COMM_SUCCESS 0

#define MAKE_COMMON_ERROR(errId) MAKE_ERROR(ERR_SEV_ERROR, COMMON_MODULE_ID, COMM_ERR_TYPE_COMM, errId)

// 通用错误码
#define ERR_COMM_MALLOC_FAILED   MAKE_COMMON_ERROR(COMM_ERR_ID_MALLOC_FAILED)
#define ERR_COMM_ARRAY_IDX_OOB   MAKE_COMMON_ERROR(COMM_ERR_ID_IDX_OUT_OF_BOUND)
#define ERR_COMM_ENUM_INVALID    MAKE_COMMON_ERROR(COMM_ERR_ID_ENUM_INVALID)
#define ERR_COMM_PARAM_NULL      MAKE_COMMON_ERROR(COMM_ERR_ID_PARAM_NULL)
#define ERR_COMM_PARAM_INVALID   MAKE_COMMON_ERROR(COMM_ERR_ID_PARAM_INVALID)
#define ERR_COMM_BUFFER_OVERFLOW MAKE_COMMON_ERROR(COMM_ERR_ID_BUFFER_OVERFLOW)
#define ERR_COMM_SYSTEM_ERROR    MAKE_COMMON_ERROR(COMM_ERR_ID_SYSTEM_ERROR)
#define ERR_COMM_GENERIC_ERROR   MAKE_COMMON_ERROR(COMM_ERR_ID_GENERIC_ERROR)
#define ERR_COMM_UNKNOWN_ERROR   MAKE_COMMON_ERROR(COMM_ERR_ID_UNKNOWN_ERROR)

typedef enum {
    DT_ERR_ID_MONTH_INVALID = 1,
    DT_ERR_ID_WEEKDAY_INVALID,
    DT_ERR_ID_TIMEZONE_INVALID,
    DT_ERR_ID_TIMESTAMP_INVALID
} DateTimeErrId;

#define MAKE_DATE_TIME_ERROR(errId) MAKE_ERROR(ERR_SEV_ERROR, COMMON_MODULE_ID, COMM_ERR_TYPE_DATE_TIME, errId)
// datetime
#define ERR_COMM_MONTH_INVALID     MAKE_DATE_TIME_ERROR(DT_ERR_ID_MONTH_INVALID)
#define ERR_COMM_WEEKDAY_INVALID   MAKE_DATE_TIME_ERROR(DT_ERR_ID_WEEKDAY_INVALID)
#define ERR_COMM_TIMEZONE_INVALID  MAKE_DATE_TIME_ERROR(DT_ERR_ID_TIMEZONE_INVALID)
#define ERR_COMM_TIMESTAMP_INVALID MAKE_DATE_TIME_ERROR(DT_ERR_ID_TIMESTAMP_INVALID)

typedef enum {
    FS_ERR_ID_NOT_FOUND = 1,      ///< 路径不存在
    FS_ERR_ID_PERMISSION_DENIED,  ///< 权限不足
    FS_ERR_ID_PATH_TOO_LONG,      ///< 路径长度超过系统限制
    FS_ERR_ID_ALREADY_EXISTS,     ///< 目标路径已存在
    FS_ERR_ID_NOT_DIRECTORY,      ///< 路径指向的不是目录
    FS_ERR_ID_NOT_FILE,           ///< 路径指向的不是文件
    FS_ERR_ID_IO_ERROR,           ///< I/O操作错误
    FS_ERR_ID_DIR_NOT_EMPTY,      ///< 文件夹非空
    FS_ERR_ID_PATH_INVALID,       ///< 路径不合法
    FS_ERR_ID_SHARING_VIOLATION,  ///< 文件共享冲突
    FS_ERR_ID_IS_A_DIRECTORY,     ///< 目标是文件夹
} FileSysErrId;

#define MAKE_FILESYS_ERROR(errId) MAKE_ERROR(ERR_SEV_ERROR, COMMON_MODULE_ID, COMM_ERR_TYPE_FILESYSTEM, errId)

#define ERR_COMM_NOT_FOUND         MAKE_FILESYS_ERROR(FS_ERR_ID_NOT_FOUND)
#define ERR_COMM_PERMISSION_DENIED MAKE_FILESYS_ERROR(FS_ERR_ID_PERMISSION_DENIED)
#define ERR_COMM_PATH_TOO_LONG     MAKE_FILESYS_ERROR(FS_ERR_ID_PATH_TOO_LONG)
#define ERR_COMM_ALREADY_EXISTS    MAKE_FILESYS_ERROR(FS_ERR_ID_ALREADY_EXISTS)
#define ERR_COMM_NOT_DIRECTORY     MAKE_FILESYS_ERROR(FS_ERR_ID_NOT_DIRECTORY)
#define ERR_COMM_NOT_FILE          MAKE_FILESYS_ERROR(FS_ERR_ID_NOT_FILE)
#define ERR_COMM_IO_ERROR          MAKE_FILESYS_ERROR(FS_ERR_ID_IO_ERROR)
#define ERR_COMM_DIR_NOT_EMPTY     MAKE_FILESYS_ERROR(FS_ERR_ID_DIR_NOT_EMPTY)
#define ERR_COMM_PATH_INVALID      MAKE_FILESYS_ERROR(FS_ERR_ID_PATH_INVALID)
#define ERR_COMM_SHARING_VIOLATION MAKE_FILESYS_ERROR(FS_ERR_ID_SHARING_VIOLATION)
#define ERR_COMM_IS_A_DIRECTORY    MAKE_FILESYS_ERROR(FS_ERR_ID_IS_A_DIRECTORY)

typedef enum {
    CTN_ERR_ID_OVERFLOW = 1,  ///< 路径不存在
} ContainerErrId;

#define MAKE_CONTAINER_ERROR(errId) MAKE_ERROR(ERR_SEV_ERROR, COMMON_MODULE_ID, COMM_ERR_TYPE_CONTAINER, errId)

#define ERR_COMM_CTN_OVERFLOW MAKE_CONTAINER_ERROR(CTN_ERR_ID_OVERFLOW)

static inline const char *GetCommErrorMsg(ErrorCode errCode)
{
    switch (errCode) {
        // Success.
        case ERR_COMM_SUCCESS:
            return "success";
        // Common error code.
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
            return "Unkown error";
        // Date time error code.
        case ERR_COMM_MONTH_INVALID:
            return "Month invalid";
        case ERR_COMM_WEEKDAY_INVALID:
            return "Weekday invalid";
        case ERR_COMM_TIMEZONE_INVALID:
            return "Time zone invalid";
        case ERR_COMM_TIMESTAMP_INVALID:
            return "Timestamp invalid";
        // Filesystem error code.
        case ERR_COMM_NOT_FOUND:
            return "Target not found";
        case ERR_COMM_PERMISSION_DENIED:
            return "Permission denied";
        case ERR_COMM_PATH_TOO_LONG:
            return "Path too long";
        case ERR_COMM_ALREADY_EXISTS:
            return "File already exists";
        case ERR_COMM_NOT_DIRECTORY:
            return "Target not directory";
        case ERR_COMM_NOT_FILE:
            return "Target not file";
        case ERR_COMM_IO_ERROR:
            return "IO error";
        case ERR_COMM_DIR_NOT_EMPTY:
            return "Directory not empty";
        case ERR_COMM_PATH_INVALID:
            return "Path invalid";
        case ERR_COMM_SHARING_VIOLATION:
            return "Sharing violation";
        case ERR_COMM_IS_A_DIRECTORY:
            return "Target is a directory";
        // Container error code.
        case ERR_COMM_CTN_OVERFLOW:
            return "Container overflow";
        // Unknown error code.
        default:
            return "Unkown error code";
    };
}

#endif  // COMMON_CONSTANTS_ERROR_CODE_CONSTANTS_H
