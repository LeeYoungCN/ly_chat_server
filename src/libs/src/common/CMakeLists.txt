set(LIB_NAME common)
list(APPEND LIB_LIST ${LIB_NAME})
set(LIB_LIST "${LIB_LIST}" CACHE STRING "" FORCE)

add_library(${LIB_NAME} SHARED)
file(GLOB_RECURSE SRC_LIST "*.cpp")
target_sources(${LIB_NAME} PRIVATE ${SRC_LIST})
