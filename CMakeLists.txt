cmake_minimum_required(VERSION 3.16)

# see https://vcpkg.io/en/docs/users/buildsystems/cmake-integration.html#visual-studio if using visual studio
# On Linux-based distrubtions, use your package manager for dependeices

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# Option support (faster build times by disabling features)
option(FAL_TESTS "Should we enable unit test support?" ON)
option(FAL_DOCS "Should we attempt to build documentation?" ON)

# Project details start here
project(M_BossFight
	VERSION 0.1
	DESCRIPTION "A multiplayer boss fight game. Begin..."
	LANGUAGES CXX C
	)

# integration for command-line users
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(project)
add_subdirectory( src )

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)

IF(APPLE)
	SET(CMAKE_CXX_LINK_FLAGS "-framework OpenGL -framework GLUT")
	SET(CMAKE_CXX_LINK_FLAGS "-framework OpenGL -framework GLUT")
ENDIF(APPLE)

set(INCLUDE_DIRS ${OPENGL_INCLUDE_DIR} ${GLUT_INCLUDE_DIR})
set(LIBRARIES ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

target_include_directories(
		project
		PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
		$<INSTALL_INTERFACE:include/>
)
target_include_directories(
		project
		PRIVATE
		/opt/homebrew/Cellar/sdl2/2.28.5/include/SDL2
		/opt/homebrew/Cellar/glew/2.2.0_1/include/GL
)

target_link_libraries(
		project
		PRIVATE
		/opt/homebrew/Cellar/sdl2/2.28.5/lib/libSDL2.dylib
		/opt/homebrew/Cellar/glew/2.2.0_1/lib/libGLEW.dylib
)



