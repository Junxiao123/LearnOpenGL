cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(HelloShader)

set(MAIN_PATH ${PROJECT_SOURCE_DIR})
configure_file(src/env.h.in ../src/env.h)

if(POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

find_package(Glog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

message(WARNING, "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

add_library(libcommon STATIC
  src/common/glad.c
  src/common/window.cpp
  src/common/shader.cpp
)
target_include_directories(libcommon
  PUBLIC
  ${PROJECT_SOURCE_DIR}/src
)

add_executable(${PROJECT_NAME}
  src/main.cpp
)
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} libcommon glfw glog)

# practice1
add_executable(practice1
  src/practice1.cpp
)
target_link_libraries(practice1 ${OPENGL_LIBRARIES} libcommon glfw glog)

# practice2
add_executable(practice2
  src/practice2.cpp
)
target_link_libraries(practice2 ${OPENGL_LIBRARIES} libcommon glfw glog)

# uniform
add_executable(uniform
  src/uniform.cpp
)
target_link_libraries(uniform ${OPENGL_LIBRARIES} glfw glog libcommon)
