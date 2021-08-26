set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


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
  )

target_link_libraries(unit_tests LINK_PRIVATE
  gtest
  fmt
  ${CMAKE_THREAD_LIBS_INIT})

target_include_directories(
  unit_tests PUBLIC
  gtest
  fmt
  ${NAMEDTYPE_INCLUDE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  )

add_test(
  NAME unit_tests
  COMMAND
  ${CMAKE_BINARY_DIR}/test/unit_tests
  ${CMAKE_CURRENT_SOURCE_DIR}/data/
  )


add_dependencies(unit_tests gtest-project)
