cmake_minimum_required(VERSION 3.10)

project(rbrouting LANGUAGES CXX)

find_package(OpenMP)
find_package(pybind11)

add_library(teexgraphlib
  src/BDGraph.cpp
  src/CenGraph.cpp
  src/Graph.cpp
  src/Timer.cpp
)
target_include_directories(teexgraphlib PUBLIC include/)
target_compile_options(teexgraphlib PRIVATE -Wall -pedantic)
target_link_libraries(teexgraphlib PRIVATE OpenMP::OpenMP_CXX)
set_property(TARGET teexgraphlib PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET teexgraphlib PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(teexgraph
  src/main.cpp
  src/examples.cpp
)
target_include_directories(teexgraph PUBLIC src/)
target_compile_options(teexgraph PRIVATE -Wall -pedantic)
target_link_libraries(teexgraph PRIVATE  teexgraphlib)

if(pybind11_FOUND)
  pybind11_add_module(pyteexgraph
    src/python_wrapper.cpp
  )
  target_link_libraries(pyteexgraph PRIVATE teexgraphlib)
else()
  message(WARNING "Could not find PyBind11; not building Python extension")
endif()
