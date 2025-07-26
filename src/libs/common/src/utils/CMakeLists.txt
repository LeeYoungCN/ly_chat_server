set(LIB_NAME utils)

list(APPEND LIB_LIST ${LIB_NAME})
set(LIB_LIST "${LIB_LIST}" CACHE STRING "" FORCE)

add_library(${LIB_NAME} SHARED)

add_subdirectory(src/platform)

target_include_directories(${LIB_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # 编译时路径
        $<INSTALL_INTERFACE:include>                            # 安装时路径
)
set_target_properties(${LIB_NAME} PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON  # 自动导出所有符号
)
target_link_libraries(${LIB_NAME} PRIVATE common)
