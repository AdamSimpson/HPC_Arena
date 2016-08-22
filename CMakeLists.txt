cmake_minimum_required(VERSION 3.1)
project(HPC-arena)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/CMake/)

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED TRUE)

file(GLOB SRC_FILES src/*.cpp src/*.h src/*/*.cpp src/*/*.h)
add_executable(arena ${SRC_FILES} src/Components/collision_bounds.h)

file(COPY resources DESTINATION .)

find_package(SFML COMPONENTS graphics window system)

include_directories("src")
include_directories(${SFML_INCLUDE_DIR})

target_link_libraries(arena ${SFML_LIBRARIES})