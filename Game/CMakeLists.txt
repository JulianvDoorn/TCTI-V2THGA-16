cmake_minimum_required(VERSION 3.9)
project(Game)

set(CMAKE_CXX_STANDARD 17)

add_executable(Game
        main.cpp )

target_link_libraries(Game sfml-graphics sfml-window sfml-audio sfml-network sfml-system)