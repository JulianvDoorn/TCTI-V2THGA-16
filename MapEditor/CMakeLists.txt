cmake_minimum_required(VERSION 3.8)
project(MapEditor)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp SFMLWidget.hpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM3 REQUIRED gtkmm-3.0)

include_directories(${GTKMM3_INCLUDE_DIRS})
link_directories(${GTKMM3_LIBRARY_DIRS})

add_executable(MapEditor ${SOURCE_FILES})
target_link_libraries(MapEditor sfml-graphics sfml-window sfml-audio sfml-network sfml-system ${GTKMM3_LIBRARIES})