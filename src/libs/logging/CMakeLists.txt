set(LIB_NAME logging)
list(APPEND LIB_LIST ${LIB_NAME})
set(LIB_LIST "${LIB_LIST}" CACHE STRING "" FORCE)

add_library(${LIB_NAME} SHARED)

file(GLOB_RECURSE SRC_LIST src "*.cpp")
target_sources(${LIB_NAME} PRIVATE ${SRC_LIST})

target_include_directories(${LIB_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>  # 编译时路径
        $<INSTALL_INTERFACE:include>                            # 安装时路径
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src                # 内部头文件路径
)

set_target_properties(${LIB_NAME} PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON  # 自动导出所有符号
)
target_link_libraries(${LIB_NAME} PRIVATE common)

if (MINGW)
    target_link_libraries(${LIB_NAME} PRIVATE pthread)
endif()
