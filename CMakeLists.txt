cmake_minimum_required(VERSION 3.20.0)
project(learnGL)

find_package(glfw3 3.4 REQUIRED)

set(SOURCES main.cpp glad.c shader.h camera.h stb_image.h nuklear.h)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(app ${SOURCES})

target_link_libraries(app)
target_link_libraries(app glfw)