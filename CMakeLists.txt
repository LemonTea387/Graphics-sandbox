cmake_minimum_required(VERSION 3.20)

# Set C++ version
set(CMAKE_CXX_STANDARD 23)

project(
  OpenGLSandbox
  VERSION 0.0.1
  DESCRIPTION "A place where I keep track of things to play around with opengl."
  LANGUAGES CXX C

)

find_package(OpenGL REQUIRED)

include(FetchContent)
# GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG aa80d24d01d150e26d664ff2d5da22e95db6bb82 # Release 3.3.8
)

FetchContent_Declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_PROGRESS TRUE
)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")

FetchContent_MakeAvailable(glfw glm)

# GLAD
file(GLOB GLAD_SRC ${CMAKE_SOURCE_DIR}/lib/glad/src/gl.c)

# IMGUI
file(GLOB IMGUI_SRC 
        ${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/misc/cpp/imgui_stdlib.*
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_glfw.cpp
)

# External Dependencies
set(DEPS ${GLAD_SRC} ${IMGUI_SRC})
set(DEPS_INCLUDE_DIR 
  "${CMAKE_SOURCE_DIR}/lib/imgui/"
  ${glm_SOURCE_DIR}
  ${OPENGL_INCLUDE_DIR}
  # STBIMAGE
  "${CMAKE_SOURCE_DIR}/lib/stb_image"
  # GLAD
  "${CMAKE_SOURCE_DIR}/lib/glad/include"
)

# Project Setup
set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/*.c
	${PROJECT_SOURCE_DIR}/*.cpp)

add_executable(${PROJECT_NAME} 
        ${DEPS}
        ${SOURCE_FILES})
# Maybe we want to do this in a cmake preset.
target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -pedantic)
target_include_directories(${PROJECT_NAME} PRIVATE
  ${DEPS_INCLUDE_DIR}
  ${PROJECT_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw)
