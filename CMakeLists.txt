cmake_minimum_required(VERSION 3.13)
project(ParticleWater)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O2 -Wall")

find_package(GLUT)
find_package(OpenGL)

file(GLOB SOURCE_FILES *.cpp glcpp/*.cpp)
add_executable(ParticleWater ${SOURCE_FILES})
target_link_libraries(ParticleWater ${GLUT_LIBRARY})
target_link_libraries(ParticleWater ${OPENGL_LIBRARY})

add_compile_definitions(GL_SILENCE_DEPRECATION)
