cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS -O3)

set(TOP_PROJECT_NAME "NZE")
project(${TOP_PROJECT_NAME})

include_directories(include)

add_executable(seq SeqSketch/experiment.cpp)
# add_executable(embed EmbedSketch/experiment.cpp)
