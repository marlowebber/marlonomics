# root level cmakelists.txt for deepsea project 

cmake_minimum_required(VERSION 3.16)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

project(game)

# To make a debug build
set(CMAKE_CXX_FLAGS "-g")

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem thread)

# Define the framework files.
set(SRCS
	src/marlonomics.cpp
	name-generator/dasmig/namegen.hpp
)

link_directories(
	${Boost_LIBRARY_DIRS}
)

include_directories (
	${SDL2_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

add_executable(game ${SRCS} )

target_link_libraries (
	game
	${SDL2_LIBRARIES}
	${OPENGL_LIBRARIES}
	${Boost_LIBRARIES}
)


