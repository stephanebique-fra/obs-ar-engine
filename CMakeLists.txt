cmake_minimum_required(VERSION 3.22)

project(OBSAREngine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

include_directories(include)

add_executable(obs-ar-engine
    src/main.cpp
    src/Application.cpp
    src/Window.cpp
)

target_link_libraries(obs-ar-engine PRIVATE SDL3::SDL3)