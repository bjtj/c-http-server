add_library(chttpserver SHARED
  header.c
  request.c
  response.c
  server.c
  status_code.c)
target_include_directories(chttpserver PRIVATE .)
target_link_libraries(chttpserver PkgConfig::LIBOSL)
install(TARGETS chttpserver DESTINATION lib)
install(FILES
  header.h
  request.h
  response.h
  server.h
  status_code.h
  DESTINATION include)


if (MSVC)
  target_compile_options(chttpserver PRIVATE /W4 /WX)
else()
  target_compile_options(chttpserver PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
