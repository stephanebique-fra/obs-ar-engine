cmake_minimum_required(VERSION 3.22)

project(OBSAREngine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)

include_directories(include)
include_directories(third_party)

add_executable(obs-ar-engine
    src/main.cpp
    src/Application.cpp
    src/Court.cpp
    src/VideoSource.cpp
    src/Window.cpp
    src/graphics/Image.cpp
    src/graphics/Texture.cpp
    src/Renderer.cpp
    src/Camera2D.cpp
)

target_link_libraries(obs-ar-engine PRIVATE SDL3::SDL3)

add_custom_command(TARGET obs-ar-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:obs-ar-engine>/assets
)
