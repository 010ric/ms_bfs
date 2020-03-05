# ---------------------------------------------------------------------------
# GraphTraversal
# ---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.7)
project(graphtraversal)

# bringing the headers into the project
include_directories(include)

# ---------------------------------------------------------------------------
# Compiler
# ---------------------------------------------------------------------------
file(GLOB DATA_READER "src/data_reader/*.cpp")
file(GLOB DATA_WRITER "src/data_writer/*.cpp")
file(GLOB WEBGRAPH "./webgraph.h")
file(GLOB BFS "src/bfs/*.cpp")
file(GLOB MS_BFS "src/ms_bfs/*.cpp")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_executable(data_reader ${DATA_READER})
add_executable(data_writer ${DATA_WRITER})
add_executable(bfs ${BFS} ${WEBGRAPH})
add_executable(ms_bfs ${MS_BFS} ${WEBGRAPH})
add_executable(basic_benchmark src/executor/basic_benchmark.cpp)

# set_target_properties(ms_bfs.cpp PROPERTIES  CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS ON)
# set_target_properties(basic_benchmark.cpp PROPERTIES  CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS ON)

# find_package(GSL REQUIRED)
# target_link_libraries(basic_benchmark GSL::gsl GSL::gslcblas)