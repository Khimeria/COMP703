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

add_executable(project
		src/shader.cpp
		src/camera.cpp
		src/mesh.cpp
		src/model.cpp
		include/project/shader.h
		include/project/camera.h
		include/project/mesh.h
		include/project/model.h
		include/project/main.hpp
		include/project/light.h
		src/light.cpp
		include/project/gameObject/game_object.h
        include/project/gameObject/primitive_go.h
		include/project/renderManager.h
		include/project/world.h
		src/renderManager.cpp
        include/project/gameObject/model_go.h
		src/playerController.cpp
		include/project/playerController.h
		include/project/iInputController.h
		include/project/iDestroyable.h
)
add_subdirectory( src )

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLUT REQUIRED)
find_package(GLEW REQUIRED)
find_package(PNG REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

IF(APPLE)
	SET(CMAKE_CXX_LINK_FLAGS "-framework OpenGL -framework GLUT")
	SET(CMAKE_CXX_LINK_FLAGS "-framework OpenGL -framework GLUT")
	set(glm_DIR /opt/homebrew/Cellar/glm/0.9.9.8/lib/cmake/glm)
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
		/opt/homebrew/Cellar/sdl_image/1.2.12_10/include/SDL
		/opt/homebrew/Cellar/glew/2.2.0_1/include/GL
		/opt/homebrew/Cellar/glm/0.9.9.8/include/glm
		/opt/homebrew/Cellar/assimp/5.3.1/include
)

target_link_libraries(
		project
		PRIVATE
		/opt/homebrew/Cellar/sdl2/2.28.5/lib/libSDL2.dylib
		/opt/homebrew/Cellar/glew/2.2.0_1/lib/libGLEW.dylib
		/opt/homebrew/Cellar/sdl_image/1.2.12_10/lib/libSDL_image.dylib
		/opt/homebrew/Cellar/assimp/5.3.1/lib/libassimp.dylib
)



