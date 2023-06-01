cmake_minimum_required(VERSION 3.0)
project(FirstSDL2)

# Paths

get_filename_component(ROOT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
set(HEADER_DIRS "${ROOT_DIR}/include")
set(SRC_DIR "${ROOT_DIR}/src")

set(LIB_FLAGS "-lSDL2 -lSDL2_image -lSDL2_ttf")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-parameter")

# Linux definitions

set(HEADER_DIRS ${HEADER_DIRS} "/usr/include/SDL2")
set(PLATFORM_LIB_FLAGS ${LIB_FLAGS} -lm)

message(STATUS "Building for Linux")

# General definitions for all targets

include_directories(${HEADER_DIRS})

# target main

set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/bin")
add_executable(main ${SRC_DIR}/main.cpp)
target_sources(main PRIVATE
        ${SRC_DIR}/Utils.cpp
        ${SRC_DIR}/WindowRenderer.cpp
        ${SRC_DIR}/Text.cpp
        ${SRC_DIR}/Entity.cpp
        ${SRC_DIR}/AnimatedEntity.cpp
        ${SRC_DIR}/Player.cpp
        ${SRC_DIR}/Monster.cpp
        ${SRC_DIR}/Background.cpp)
target_link_libraries(main ${PLATFORM_LIB_FLAGS})
