include(../cmake/CLI11.cmake)

function(add_cli_exe T)
  add_executable(${T} ${ARGN})

  target_link_libraries(${T} PUBLIC GraphPathFinderLib)

  target_include_directories(${T} PUBLIC
	GraphPathFinderLib 
    ${CLI11_INCLUDE_DIR})

  set_property(TARGET ${T} PROPERTY FOLDER "Examples")

  add_dependencies(${T} CLI11-project)
endfunction()

add_cli_exe(dijkstra dijkstra.cpp)
add_cli_exe(hublabels hublabels.cpp)
