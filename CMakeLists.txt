cmake_minimum_required(VERSION 3.23)

# Set C++ version
set(CMAKE_CXX_STANDARD 23)

project(
  GraphicsSandbox
  VERSION 0.0.1
  DESCRIPTION "A place where I keep track of things to play around with graphics."
  LANGUAGES CXX C

)

## TODO: Make IMGUI avail by fetchcontent
# IMGUI
file(GLOB IMGUI_SRC
        ${CMAKE_SOURCE_DIR}/lib/imgui/*.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/misc/cpp/imgui_stdlib.*
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_opengl3.cpp
        ${CMAKE_SOURCE_DIR}/lib/imgui/backends/imgui_impl_glfw.cpp
)

# External Dependencies
set(DEPS_INCLUDE_DIR
  "${CMAKE_SOURCE_DIR}/lib/imgui/"
)

# NOTE: If we move this out of this repository we'll think of proper package
# management.
add_subdirectory(lib/engine)

add_executable(${PROJECT_NAME})

# Add source files
target_sources(${PROJECT_NAME}
 PRIVATE
  FILE_SET sandbox_headers
  TYPE HEADERS
  BASE_DIRS src/
 PRIVATE
  src/main.cpp
  src/playground_app.cpp
  src/programs/triangle.cpp
  src/programs/textures.cpp
  src/programs/dvd_screensaver.cpp

  ## Imgui
  ${IMGUI_SRC}
)

# Maybe we want to do this in a cmake preset.
if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
        target_compile_options(
                ${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror
        )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  ${DEPS_INCLUDE_DIR}
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE TeaEngine
)
