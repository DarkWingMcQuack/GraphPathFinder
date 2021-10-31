cmake_minimum_required(VERSION 3.12)

#get options and compiling env
include(cmake/options.cmake)

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
include(cmake/namedtype.cmake)
include(cmake/expected.cmake)


# needed for multithreading
find_package(Threads REQUIRED)


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
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/Sortable.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/concepts/Permutable.hpp
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

  
  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/distoracle/hublabels/HubLabelLookup.hpp
  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/distoracle/hublabels/HubLabelCalculator.hpp

  ${CMAKE_CURRENT_LIST_DIR}/include/algorithms/distoracle/PHAST.hpp
  )

# add the dependencies of the target to enforce
# the right order of compiling
add_dependencies(GraphPathFinderLib fmt-project)
add_dependencies(GraphPathFinderLib tbb-project)
add_dependencies(GraphPathFinderLib namedtype-project)
add_dependencies(GraphPathFinderLib expected-lite-project)

# make headers available
target_include_directories(GraphPathFinderLib INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  fmt
  tbb
  ${NAMEDTYPE_INCLUDE_DIR}
  ${EXPECTED_LITE_INCLUDE_DIR}
  )

#link against libarys
target_link_libraries(GraphPathFinderLib INTERFACE
  ${CMAKE_THREAD_LIBS_INIT}
  fmt
  tbb)


if (BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif (BUILD_TESTS)

if (BUILD_EXAMPLES)
  add_subdirectory(examples)
endif (BUILD_EXAMPLES)
