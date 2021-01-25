add_library(chttpserver SHARED
  connection.c
  header.c
  protocol.c
  request.c
  response.c
  router.c
  server.c
  status_code.c
  transfer.c)
target_include_directories(chttpserver PRIVATE .)
target_link_libraries(chttpserver PkgConfig::LIBOSL)
install(TARGETS chttpserver DESTINATION lib)
install(FILES
  connection.h
  header.h
  protocol.h
  request.h
  response.h
  router.h
  server.h
  status_code.h
  transfer.h
  DESTINATION include)


if (MSVC)
  target_compile_options(chttpserver PRIVATE /W4 /WX)
else()
  target_compile_options(chttpserver PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
