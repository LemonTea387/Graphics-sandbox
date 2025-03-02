cmake_minimum_required(VERSION 3.16)

project(
  OpenGLSandbox
  VERSION 0.0.1
  DESCRIPTION "A place where I keep track of things to play around with opengl."
  LANGUAGES CXX
)

set(PROJECT_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

set(PROJECT_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${PROJECT_SOURCE_DIR}/*.c
	${PROJECT_SOURCE_DIR}/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${PROJECT_SOURCE_DIR}/*.h
	${PROJECT_SOURCE_DIR}/*.hpp)

# Include directories
include_directories(${PROJECT_INCLUDE_DIR})

find_package(OpenGL REQUIRED)

include(FetchContent)
# GLFW
FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG aa80d24d01d150e26d664ff2d5da22e95db6bb82 # Release 3.3.8
)
# GLAD
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
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

FetchContent_MakeAvailable(glfw glad glm)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE
  "${glm_SOURCE_DIR}"
  "${OPENGL_INCLUDE_DIR}"
  "${CMAKE_SOURCE_DIR}/include"
  "${CMAKE_SOURCE_DIR}/lib/stb_image")
target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glad)
