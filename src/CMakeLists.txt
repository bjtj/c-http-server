add_library(chttpserver SHARED
  header.c
  server.c)
target_include_directories(chttpserver PRIVATE .)
target_link_libraries(chttpserver PkgConfig::LIBOSL)
install(TARGETS chttpserver DESTINATION lib)
install(FILES
  header.h
  server.h
  DESTINATION include)


