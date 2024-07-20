cmake_minimum_required(VERSION 3.20.0)
set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_VERBOSE_MAKEFILE ON)

project(app)

find_package(glfw3 3.4 REQUIRED)
find_package(ZLIB)
find_library(ASSIMP_LIB libassimp.a ${CMAKE_SOURCE_DIR}/lib)
set(SOURCES main.cpp glad.c shader.h camera.h stb_image.h mesh.h model.h)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC assimp)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC ZLIB::ZLIB)