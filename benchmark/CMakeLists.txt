set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")

include(../cmake/flags.cmake)
include(../cmake/ccache.cmake)

# needed for multithreading
find_package(Threads REQUIRED)

add_executable(benchmarks
  main.cpp
  )

target_link_libraries(benchmarks LINK_PRIVATE
  fmt
  GraphPathFinderLib
  ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(
  benchmarks PUBLIC
  GraphPathFinderLib
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  )
