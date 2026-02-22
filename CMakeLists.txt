cmake_minimum_required(VERSION 4.0)
project(sdl2)

set(CMAKE_CXX_STANDARD 20)

add_executable(sdl2
        Sprites.h
        sprite.cpp
        block.h
        block.cpp
        program.cpp
        SoundManager.h
        SoundManager.cpp)

find_package(SDL2_mixer REQUIRED)
include_directories(${SDL2_MIXER_INCLUDE_DIRS})
target_link_libraries(sdl2 ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
target_link_libraries(${PROJECT_NAME} -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)
