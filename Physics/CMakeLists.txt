cmake_minimum_required(VERSION 3.8)
project(Physics)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp BallBody.hpp RectangleBody.hpp Body.hpp VectorMath.hpp)

add_executable(Physics ${SOURCE_FILES})
target_link_libraries(Physics sfml-graphics sfml-window sfml-audio sfml-network sfml-system)