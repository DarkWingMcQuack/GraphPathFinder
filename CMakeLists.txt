cmake_minimum_required(VERSION 3.14)

#get options and compiling env
include(cmake/options.cmake)
include(cmake/linker.cmake)
include(cmake/ccache.cmake)

project(GraphPathFinder)
project(GraphPathFinder LANGUAGES CXX)

#set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

############################
###get external projects
############################
include(cmake/fmt.cmake)
include(cmake/tbb.cmake)
include(cmake/CLI11.cmake)
include(cmake/progress.cmake)
include(cmake/nlohmann.cmake)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# needed for multithreading
find_package(Threads REQUIRED)

include(cmake/flags.cmake)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts")

#############################################
## library with all the logic which
## can later be easily linked against test
#############################################
add_library(GraphPathFinderLib INTERFACE) 

target_sources(GraphPathFinderLib
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/include/concept/Graph.hpp
  )

# add the dependencies of the target to enforce
# the right order of compiling
add_dependencies(GraphPathFinderLib fmt-project)
add_dependencies(GraphPathFinderLib nlohmann-project)
add_dependencies(GraphPathFinderLib tbb-project)
add_dependencies(GraphPathFinderLib CLI11-project)
add_dependencies(GraphPathFinderLib progress-cpp-project)

# make headers available
target_include_directories(GraphPathFinderLib INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  fmt
  tbb
  ${CLI11_INCLUDE_DIR}
  ${PROGRESS_CPP_INCLUDE_DIR}
  ${NLOHMANN_INCLUDE_DIR}
  )

#link against libarys
target_link_libraries(GraphPathFinderLib INTERFACE
  fmt
  tbb)

target_link_libraries(GraphPathFinderLib INTERFACE
  ${CMAKE_THREAD_LIBS_INIT})


###############################
## THE ACTUAL BINARY
###############################
add_executable(GraphPathFinder src/main.cpp)

# make headers available
target_include_directories(GraphPathFinder PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  fmt
  tbb
  ${CLI11_INCLUDE_DIR}
  ${PROGRESS_CPP_INCLUDE_DIR}
  ${NLOHMANN_INCLUDE_DIR}
  )

#link against libarys
target_link_libraries(GraphPathFinder LINK_PUBLIC
  GraphPathFinderLib
  fmt
  tbb
  ${CMAKE_THREAD_LIBS_INIT})

# add the dependencies of the target to enforce
# the right order of compiling
add_dependencies(GraphPathFinder GraphPathFinderLib)
