set(SERVER_NAME ly_chat_server)

add_executable(${SERVER_NAME})

file(GLOB_RECURSE SRC_LIST "*.cpp")
target_sources(${SERVER_NAME} PRIVATE ${SRC_LIST})
target_link_libraries(${SERVER_NAME} PRIVATE ${LIB_LIST})

install(TARGETS ${SERVER_NAME} COMPONENT runtime)
