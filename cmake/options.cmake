option(USE_CLANG "build application with clang" OFF)

if(USE_CLANG)
  SET(CMAKE_C_COMPILER    "clang")
  SET(CMAKE_CXX_COMPILER  "clang++")
  SET(CMAKE_AR            "llvm-ar")
  SET(CMAKE_NM            "llvm-nm")
  SET(CMAKE_OBJDUMP       "llvm-objdump")
  SET(CMAKE_RANLIB        "llvm-ranlib")
endif(USE_CLANG)

option(BUILD_TESTS "build tests" ON)
option(BUILD_TRAVIS_TEST "builds tests for travis leaving those with huge memory requirements of" OFF)
option(BUILD_EXAMPLES "build examples" ON)
