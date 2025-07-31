set(TEST_RUNTIME_NAME ly_chat_test)
add_executable(${TEST_RUNTIME_NAME})

file(GLOB_RECURSE SRC_LIST "*.cpp")

target_sources(${TEST_RUNTIME_NAME} PRIVATE ${SRC_LIST})

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${CMAKE_THIRD_PARTY_PATH}/googletest")

find_package(GTest QUIET)

if (NOT GTest_FOUND)
    include(${CMAKE_SOURCE_DIR}/cmake/fetch_content.cmake)
endif()

target_link_libraries(${TEST_RUNTIME_NAME} ${LIB_LIST} GTest::gtest)

# 添加ctest测试
add_test(NAME TEST_ALL COMMAND ${TEST_RUNTIME_NAME})
set_tests_properties(TEST_ALL PROPERTIES TIMEOUT 10)
