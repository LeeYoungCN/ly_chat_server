cmake_minimum_required(VERSION 3.14)
project(ly_chat_server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)
set(CMAKE_VERBOSE_MAKEFILE          OFF)
option(BUILD_TESTS "Build and run tests" ON)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g3 -O0")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g3 -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
add_compile_options("-Wall" "-Wconversion" "-Wextra" "-pedantic")


if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Default CMAKE_BUILD_TYPE Debug" FORCE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
else()
    add_compile_definitions(NDEBUG_MODE)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/get_cmake_preset.cmake)

if (${CMAKE_INSTALL_PREFIX} STREQUAL "C:/Program Files (x86)/${PROJECT_NAME}" OR
    ${CMAKE_INSTALL_PREFIX} STREQUAL "/usr/local")
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/out/install/${CMAKE_PRESET}" CACHE STRING "Default CMAKE_INSTALL_PREFIX out/install/${CMAKE_PRESET}" FORCE)
endif()

if (NOT CMAKE_THIRD_PARTY_PATH)
    set(CMAKE_THIRD_PARTY_PATH "${CMAKE_SOURCE_DIR}/third_party/${CMAKE_PRESET}" CACHE STRING "Default CMAKE_THIRD_PARTY_PATH third_party" FORCE)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/cmake_configure_info.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(src)
