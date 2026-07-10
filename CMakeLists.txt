cmake_minimum_required(VERSION 3.22)

project(OBSAREngine VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf REQUIRED)
find_package(OpenCV REQUIRED)
find_library(OpenGL_LIBRARY OpenGL)

include_directories(include)
include_directories(third_party)


add_executable(obs-ar-engine
    third_party/glad/src/gl.c

    src/main.cpp
    src/Application.cpp
    src/Calibration.cpp
    src/Court.cpp
    src/VideoSource.cpp
    src/Window.cpp
    src/graphics/Image.cpp
    src/graphics/Texture.cpp
    src/Renderer.cpp
    src/Camera2D.cpp
    src/Homography.cpp
    src/FibaCourt.cpp
    src/Logo.cpp
)

target_include_directories(obs-ar-engine PRIVATE
    third_party/glad/include
)

target_link_libraries(obs-ar-engine PRIVATE
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    ${OpenCV_LIBS}
    ${OpenGL_LIBRARY}
)

add_custom_command(TARGET obs-ar-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:obs-ar-engine>/assets
)


