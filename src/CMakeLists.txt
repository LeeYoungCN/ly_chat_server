set(LIB_LIST "" CACHE STRING "")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/libs/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/libs)

message(STATUS "LIB_LIST: ${LIB_LIST}")
add_subdirectory(server)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
