cmake_minimum_required(VERSION 4.0)
project(FOP_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(FOP_engine main.cpp
        blocks.h
        Executor.h
        Executor.cpp
        sprites.h)
