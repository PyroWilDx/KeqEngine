cmake_minimum_required(VERSION 3.8)
project(Keqing)

# Paths

get_filename_component(ROOT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
set(HEADER_DIRS "${ROOT_DIR}/include")
set(SRC_DIR "${ROOT_DIR}/src")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")

if (WIN32)
    set(LIB_FLAGS -LC:/C/SDL2/lib -lmingw32 -lrpcrt4 -lSDL2main -lSDL2.dll -lSDL2_image -lSDL2_ttf -lm)

    set(SDL2 "C:/C/SDL2")
    set(RPCRT4 "C:/C/rpcrt4")

    set(HEADER_DIRS ${HEADER_DIRS} ${SDL2}/include)
    set(HEADER_DIRS ${HEADER_DIRS} ${RPCRT4}/include)

    set(PLATFORM_LIB_FLAGS ${LIB_FLAGS}
            ${SDL2}/lib/libSDL2main.a
            ${SDL2}/lib/libSDL2.dll.a
            ${SDL2}/lib/libSDL2_image.a
            ${SDL2}/lib/libSDL2_ttf.a
            ${RPCRT4}/lib/librpcrt4.a)

    set(BOOST_ROOT "C:/C/boost")

    message(STATUS "Building for Windows")

elseif (UNIX AND NOT APPLE)
    set(LIB_FLAGS "-lSDL2 -lSDL2_image -lSDL2_ttf")

    set(HEADER_DIRS ${HEADER_DIRS} "/usr/include/SDL2")
    set(PLATFORM_LIB_FLAGS ${LIB_FLAGS})

#    set(BOOST_ROOT "C:/C/boost") # TODO

    message(STATUS "Building for Linux")

endif ()

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

include_directories(${HEADER_DIRS})

# Main

set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/bin")
add_executable(main ${SRC_DIR}/Main/Main.cpp)

file(GLOB_RECURSE ALL_SRC
        LIST_DIRECTORIES true
        "${SRC_DIR}/*.cpp"
        "${SRC_DIR}/*.c"
        "${SRC_DIR}/**/*.cpp"
        "${SRC_DIR}/**/*.c")

target_sources(main PRIVATE ${ALL_SRC})
target_link_libraries(main ${PLATFORM_LIB_FLAGS})
