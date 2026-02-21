cmake_minimum_required(VERSION 4.0)
project(sdl2)

set(CMAKE_CXX_STANDARD 20)

add_executable(sdl2
        Sprites.h
        sprite.cpp
        block.h
        block.cpp
        program.cpp
        Executor.h
        Executor.cpp)
target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)
