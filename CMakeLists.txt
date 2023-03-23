cmake_minimum_required(VERSION 3.5)

project(BronKerbosh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(GRAPHVIZ_DIR "${CMAKE_SOURCE_DIR}/Graphviz")

add_executable(BronKerbosh main.cpp graph.h graph.cpp graphviz_utility.h graphviz_utility.cpp )

#file(GLOB GVC_LIBS "${GRAPHVIZ_DIR}/lib/*.lib")
#target_link_libraries(${CMAKE_PROJECT_NAME} ${GVC_LIBS})
#include_directories("${GRAPHVIZ_DIR}/include/graphviz")
