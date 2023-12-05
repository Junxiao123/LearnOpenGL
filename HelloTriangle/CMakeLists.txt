cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

project(HelloTriangle)

if (POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

find_package(Glog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

message(WARNING, "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB project_file
  src/glad.c
  src/main.cpp)

add_executable(${PROJECT_NAME} ${project_file})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glfw glog)
