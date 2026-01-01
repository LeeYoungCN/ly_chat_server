set(SERVER_NAME ly_chat_server)

add_executable(${SERVER_NAME})

file(GLOB_RECURSE SRC_LIST "*.cpp")
target_sources(${SERVER_NAME} PRIVATE ${SRC_LIST})
target_link_libraries(${SERVER_NAME} PRIVATE ${LIB_LIST} vfw32.lib user32.lib gdi32.lib ole32.lib)

install(TARGETS ${SERVER_NAME} COMPONENT runtime)
