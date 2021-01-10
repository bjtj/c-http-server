cmake_minimum_required(VERSION 3.5)
project(chttpserver LANGUAGES C VERSION "1.0.0")

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBOSL REQUIRED IMPORTED_TARGET osl)
include(CTest)

find_package (Threads REQUIRED)
set(CMAKE_C_FLAGS -pthread)
set(CMAKE_C_FLAGS -pthread)

# 
add_subdirectory(src)
enable_testing()
add_subdirectory(test)

# pkgconfig
set(DEST_DIR "${CMAKE_INSTALL_PREFIX}")
set(PRIVATE_LIBS "-lchttpserver")
configure_file(chttpserver.pc.in chttpserver.pc @ONLY)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/chttpserver.pc" DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig")
