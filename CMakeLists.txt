cmake_minimum_required(VERSION 4.0)
project(FOP_Project)

set(CMAKE_CXX_STANDARD 20)

set(SDL2_PATH C:/MinGW/SDL2)

include_directories(include)
include_directories(${SDL2_PATH}/include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(FOP_Project ${SOURCES})

link_directories(&{SDL2_PATH}/lib/x64)

target_link_libraries(FOP_Project SDL2main SDL2)
