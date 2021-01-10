include_directories(${PROJECT_SOURCE_DIR}/src)

find_package (Threads)

find_program( MEMORYCHECK_COMMAND valgrind)
set( MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")

set( MEMORYCHECK_SUPPRESSIONS_FILE "${PROJECT_SOURCE_DIR}/valgrind_suppress.txt")


set(MYLIBS chttpserver ${CMAKE_DL_LIBS} ${CMAKE_THREAD_LIBS_INIT})
file(GLOB TEST_SOURCES test_*.c)
foreach( testsourcefile ${TEST_SOURCES} )
  get_filename_component( testname ${testsourcefile} NAME_WLE )
  add_executable( ${testname} ${testsourcefile} )
  target_link_libraries( ${testname} ${MYLIBS} )
  if (MSVC)
    target_compile_options(${testname} PRIVATE /W4 /WX)
  else()
    target_compile_options(${testname} PRIVATE -Wall -Wextra -pedantic -Werror)
  endif()
  add_test( NAME ${testname} COMMAND ${testname} )
endforeach( testsourcefile ${TEST_SOURCES} )
