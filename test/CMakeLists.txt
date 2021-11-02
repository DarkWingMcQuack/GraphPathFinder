set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")

include(../cmake/flags.cmake)
include(../cmake/gtest.cmake)
include(../cmake/ccache.cmake)

# needed for multithreading
find_package(Threads REQUIRED)

add_executable(unit_tests
  main.cpp

  common/TokenizerTest.cpp
  common/ParsingTest.cpp
  common/BackwardEdgeViewTest.cpp

  graphs/nodes/FMINodeTest.cpp
  graphs/edges/FMIEdgeTest.cpp
  graphs/offsetarray/OffsetArrayTest.cpp

  algorithms/pathfinding/dijkstra/DijkstraTest.cpp
  algorithms/pathfinding/ch/CHDijkstraTest.cpp

  algorithms/distoracle/dijkstra/DijkstraTest.cpp

  algorithms/distoracle/ch/CHDijkstraTest.cpp

  algorithms/distoracle/hublabels/HubLabelTest.cpp

  algorithms/distoracle/PHASTTest.cpp

  utils/PermutationTest.cpp
  )

if (BUILD_TRAVIS_TEST)
  set_target_properties(unit_tests PROPERTIES COMPILE_DEFINITIONS "ONLY_TRAVIS_TESTS")
endif (BUILD_TRAVIS_TEST)

target_link_libraries(unit_tests LINK_PRIVATE
  gtest
  fmt
  GraphPathFinderLib
  ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(
  unit_tests PUBLIC
  gtest
  GraphPathFinderLib
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  )

add_test(
  NAME unit_tests
  COMMAND
  ${CMAKE_BINARY_DIR}/test/unit_tests
  ${CMAKE_CURRENT_SOURCE_DIR}/../data/
  )


add_dependencies(unit_tests gtest-project)
