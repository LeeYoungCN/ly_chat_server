set(LIB_LIST "" CACHE STRING "")
set(LIB_INC_LIST "" CACHE STRING "")
add_subdirectory(libs)

message(STATUS "LIB_LIST: ${LIB_LIST}")
add_subdirectory(server)

if (BUILD_TESTS)
    add_subdirectory(tests)
endif()
