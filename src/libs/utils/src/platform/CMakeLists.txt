file(GLOB SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")
target_sources(${LIB_NAME} PRIVATE ${SRC_LIST})

if(WIN32)
    add_subdirectory(windows)
elseif(UNIX AND NOT APPLE)
    add_subdirectory(linux)
elseif(APPLE)
    add_subdirectory(macos)
endif()


