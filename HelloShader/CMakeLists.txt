cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(HelloShader)

set(MAIN_PATH ${PROJECT_SOURCE_DIR})
configure_file(src/env.h.in ../src/env.h)

if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

find_package(Glog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

message(WARNING, "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB project_file
  src/common/glad.c
  src/common/window.cpp
  src/common/shader.cpp
  src/main.cpp)

add_executable(${PROJECT_NAME} ${project_file})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glog)
