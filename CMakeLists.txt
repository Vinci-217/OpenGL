cmake_minimum_required(VERSION 3.26)
project(OpenGL)

set(CMAKE_C_STANDARD 11)


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

add_executable(OpenGL src/glad.c main.cpp)


link_directories(${PROJECT_SOURCE_DIR}/lib)
target_link_libraries(OpenGL ${PROJECT_SOURCE_DIR}/lib/glfw3.dll)