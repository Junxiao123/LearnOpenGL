cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

project(LearnOpenGL)

set(MAIN_PATH ${PROJECT_SOURCE_DIR})
configure_file(common/env.h.in ../common/env.h)

if(POLICY CMP0072)
  set(OpenGL_GL_PREFERENCE LEGACY)
endif()

find_package(Glog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/third_party/include)

message(WARNING, "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR})

add_library(libcommon STATIC
  common/glad.c
  common/window.cpp
  common/shader.cpp
)
target_include_directories(libcommon
  PUBLIC
  ${PROJECT_SOURCE_DIR}
)

# ************************* HelloWorld **************************
add_executable(HelloWorld HelloWorld/src/main.cpp)
target_link_libraries(HelloWorld ${OPENGL_LIBRARIES} libcommon glfw glog)

# ************************* HelloTriangle **************************
add_executable(HelloTriangle HelloTriangle/src/main.cpp)
target_link_libraries(HelloTriangle ${OPENGL_LIBRARIES} libcommon glfw glog)

# ************************* HelloShader **************************
add_executable(HelloShader HelloShader/src/main.cpp)
target_link_libraries(HelloShader ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(HelloShader_practice1 HelloShader/src/practice1.cpp)
target_link_libraries(HelloShader_practice1 ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(HelloShader_practice2 HelloShader/src/practice2.cpp)
target_link_libraries(HelloShader_practice2 ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(HelloShader_uniform HelloShader/src/uniform.cpp)
target_link_libraries(HelloShader_uniform ${OPENGL_LIBRARIES} libcommon glfw glog)

# *********************** HelloTexture **************************
add_executable(HelloTexture HelloTexture/src/main.cpp)
target_link_libraries(HelloTexture ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(HelloTexture_mix HelloTexture/src/mix.cpp)
target_link_libraries(HelloTexture_mix ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(HelloTexture_practice1 HelloTexture/src/practice1.cpp)
target_link_libraries(HelloTexture_practice1 ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(HelloTexture_practice2 HelloTexture/src/practice2.cpp)
target_link_libraries(HelloTexture_practice2 ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(HelloTexture_practice3 HelloTexture/src/practice1.cpp)
target_link_libraries(HelloTexture_practice3 ${OPENGL_LIBRARIES} libcommon glfw glog)

# *********************** Transformations **************************
add_executable(Transformations Transformations/src/main.cpp)
target_link_libraries(Transformations ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(Transformations_practice2 Transformations/src/practice2.cpp)
target_link_libraries(Transformations_practice2 ${OPENGL_LIBRARIES} libcommon glfw glog)

# *********************** CoordinateSystems **************************
add_executable(CoordinateSystems CoordinateSystems/src/main.cpp)
target_link_libraries(CoordinateSystems ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(CoordinateSystems_cube CoordinateSystems/src/cube.cpp)
target_link_libraries(CoordinateSystems_cube ${OPENGL_LIBRARIES} libcommon glfw glog)

add_executable(CoordinateSystems_cube10 CoordinateSystems/src/cube10.cpp)
target_link_libraries(CoordinateSystems_cube10 ${OPENGL_LIBRARIES} libcommon glfw glog)
