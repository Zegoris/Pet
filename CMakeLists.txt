cmake_minimum_required(VERSION 3.28)

project(MyProjects LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(tamagotchi
        main.cpp
        src/types.cpp
        src/pet.cpp
        src/needs.cpp
        src/world.cpp
        src/inventory.cpp
        src/storage.cpp
        src/achievements.cpp
        src/game.cpp
)


target_sources(tamagotchi
        PRIVATE FILE_SET
        CXX_MODULES FILES
        include/types.cppm
        include/pet.cppm
        include/needs.cppm
        include/world.cppm
        include/inventory.cppm
        include/storage.cppm
        include/achievements.cppm
        include/game.cppm
)


if (MSVC)
    target_compile_options(tamagotchi PRIVATE /W4 /permissive-)
else ()
    target_compile_options(tamagotchi PRIVATE -Wall -Wextra -Wpedantic)
endif ()
