cmake_minimum_required(VERSION 3.23)

project(
TeaEngine
VERSION 0.0.1
DESCRIPTION "Some General engine I write along the way to accomodate my uses."
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
FetchContent_Declare(
	spd_log
	GIT_REPOSITORY https://github.com/gabime/spdlog.git
	GIT_TAG v1.15.2
)
# GLFW
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "Build the GLFW documentation")

FetchContent_MakeAvailable(glfw glm glad stb spd_log)

# stb_image
add_library(stb_image INTERFACE ${stb_SOURCE_DIR}/stb_image.h)
target_include_directories(stb_image INTERFACE ${stb_SOURCE_DIR})

# GLAD
glad_add_library(glad_gl_core_41
	STATIC REPRODUCIBLE LANGUAGE C API
	gl:core=4.1
	EXTENSIONS NONE
)

add_library(${PROJECT_NAME} STATIC)

# Add source files
target_sources(${PROJECT_NAME}
	PUBLIC
		FILE_SET engine_headers
		TYPE HEADERS
		BASE_DIRS include/
		FILES
		include/tea/application.hpp
		include/tea/engine_error.hpp
		include/tea/event.hpp
		include/tea/input/input.hpp
		include/tea/input/key_code.hpp
		include/tea/types.hpp
		include/tea/window.hpp
		include/tea/renderer/shader.hpp
		include/tea/renderer/texture.hpp
		include/tea/logging.hpp
	PRIVATE
		src/event.cpp
		src/input/input.cpp
		src/window.cpp
		src/renderer/shader.cpp
		src/renderer/texture.cpp
		src/logging.cpp
)
# Maybe we want to do this in a cmake preset.
if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX)
else()
	target_compile_options(
		${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror
	)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
	${DEPS_INCLUDE_DIR}
)

target_link_libraries(
${PROJECT_NAME}
PUBLIC OpenGL::GL glfw glm::glm glad_gl_core_41
PRIVATE stb_image spdlog::spdlog
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
