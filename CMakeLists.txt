cmake_minimum_required(VERSION 3.5)
project(chttpserver)

# 
add_subdirectory(src)
add_subdirectory(test)
enable_testing()
add_test(NAME mytest COMMAND Test)

include(ExternalProject)
ExternalProject_Add(osl-c
  GIT_REPOSITORY    git@github.com:bjtj/osl-c.git
  GIT_TAG           origin/release/1.0.0
)

# pkgconfig
set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
set(PRIVATE_LIBS "-lchttpserver")
configure_file(chttpserver.pc.in chttpserver.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/chttpserver.pc" DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
