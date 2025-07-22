file(GLOB LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/*")
foreach(DIR ${LIBS_DIR})
    if (IS_DIRECTORY ${DIR})
        add_subdirectory(${DIR})
    endif()
endforeach()

install(TARGETS ${LIB_LIST}
        ARCHIVE DESTINATION lib  # 静态库 (.a)
        LIBRARY DESTINATION lib  # 共享库 (.so/.dylib)
        RUNTIME DESTINATION bin  # Windows DLL
        COMPONENT libs)
