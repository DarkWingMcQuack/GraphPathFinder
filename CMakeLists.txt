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
# include(cmake/CLI11.cmake)
# include(cmake/progress.cmake)
# include(cmake/nlohmann.cmake)
include(cmake/namedtype.cmake)
include(cmake/gtest.cmake)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER}")

if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# needed for multithreading
find_package(Threads REQUIRED)

include(cmake/flags.cmake)

#############################################
## library with all the logic which
## can later be easily linked against test
#############################################
add_library(GraphPathFinderLib INTERFACE) 

target_sources(GraphPathFinderLib
  INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/BackwardConnections.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/BackwardEdges.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/DistanceOracle.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/EdgeWeights.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/Edges.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/ForwardConnections.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/NodeLevels.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/Nodes.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/Parseable.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/Path.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/PathOracle.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/SortableGraph.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/Utils.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/common/BackwardEdgeView.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/common/BasicGraphTypes.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/common/EmptyBase.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/common/Parsing.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/common/Range.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/common/Tokenizer.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/utils/MinMax.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/utils/Permutation.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/common/LevelBase.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/common/WeightBase.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/nodes/LatLngBase.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/nodes/FMINode.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/nodes/SimpleMapNode.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/edges/FMIEdge.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/edges/ShortcutBase.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/edges/SimpleEdge.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/edges/SrcBase.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/edges/TrgBase.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/edges/WeightedEdge.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/offsetarray/OffsetArray.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/offsetarray/OffsetArrayBackwardGraph.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/offsetarray/OffsetArrayForwardGraph.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/offsetarray/OffsetArrayNodes.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/offsetarray/OffsetArrayEdges.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/graphs/Path.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/parsing/offsetarray/Parser.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/pathfinding/dijkstra/DijkstraQueue.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/pathfinding/dijkstra/Dijkstra.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/distoracle/dijkstra/Dijkstra.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/distoracle/ch/CHDijkstraBackwardHelper.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/distoracle/ch/CHDijkstraForwardHelper.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/distoracle/ch/CHDijkstra.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/distoracle/PHAST.hpp
  )

# add the dependencies of the target to enforce
# the right order of compiling
add_dependencies(GraphPathFinderLib fmt-project)
# add_dependencies(GraphPathFinderLib nlohmann-project)
add_dependencies(GraphPathFinderLib tbb-project)
# add_dependencies(GraphPathFinderLib CLI11-project)
# add_dependencies(GraphPathFinderLib progress-cpp-project)
add_dependencies(GraphPathFinderLib namedtype-project)

# make headers available
target_include_directories(GraphPathFinderLib INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  fmt
  tbb
  ${CLI11_INCLUDE_DIR}
  ${NAMEDTYPE_INCLUDE_DIR}
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
  ${NAMEDTYPE_INCLUDE_DIR}
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

if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif (BUILD_TESTS)
