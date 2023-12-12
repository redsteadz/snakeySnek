cmake_minimum_required(VERSION 3.1)

project(snake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

find_package(Raylib REQUIRED)

add_executable(Snake main.cpp)

target_link_libraries(Snake PRIVATE -lraylib)
