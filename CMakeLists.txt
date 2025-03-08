cmake_minimum_required(VERSION 3.20)

# Set C++ version
set(CMAKE_CXX_STANDARD 23)

project(
  GraphicsSandbox
  VERSION 0.0.1
  DESCRIPTION "A place where I keep track of things to play around with graphics."
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
FetchContent_Declare(
        glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        GIT_TAG        v2.0.6
        SOURCE_SUBDIR  cmake
)
FetchContent_Declare(
        stb
	GIT_REPOSITORY https://github.com/nothings/stb.git
	GIT_TAG master
)

# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")

FetchContent_MakeAvailable(glfw glm glad stb)

# stb_image
add_library(stb_image INTERFACE ${stb_SOURCE_DIR}/stb_image.h)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

# GLAD
glad_add_library(glad_gl_core_41 STATIC REPRODUCIBLE LANGUAGE C API gl:core=4.1 EXTENSIONS NONE)

## TODO: Make IMGUI avail by fetchcontent
# IMGUI
file(GLOB IMGUI_SRC 
        ${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/misc/cpp/imgui_stdlib.*
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_glfw.cpp
)

# External Dependencies
set(DEPS ${IMGUI_SRC})
set(DEPS_INCLUDE_DIR 
  "${CMAKE_SOURCE_DIR}/lib/imgui/"
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
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE
  ${DEPS_INCLUDE_DIR}
  ${PROJECT_INCLUDE_DIR}
  ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL glfw glm::glm glad_gl_core_41 stb_image)
