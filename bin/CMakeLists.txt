include_directories(${PROJECT_SOURCE_DIR}/src)

find_package (Threads)

set(MYLIBS chttpserver ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
add_executable(server server.c)
target_link_libraries(server ${MYLIBS} )
