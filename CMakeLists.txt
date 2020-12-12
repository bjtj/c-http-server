cmake_minimum_required(VERSION 3.5)
project(chttpserver)

# 
add_subdirectory(src)
add_subdirectory(test)
enable_testing()
add_test(NAME mytest COMMAND Test)

include(ExternalProject)
ExternalProject_Add(oslc
  GIT_REPOSITORY    git@github.com:bjtj/osl-c.git
  GIT_TAG           1.0.0
  INSTALL_COMMAND   ""
  )

# pkgconfig
set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
set(PRIVATE_LIBS "-lchttpserver")
configure_file(chttpserver.pc.in chttpserver.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/chttpserver.pc" DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
