cmake_minimum_required(VERSION 3.8)
project(Keqing)

# Paths

get_filename_component(ROOT_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
set(SRC_DIR "${ROOT_DIR}/src")

set(HEADER_DIRS "${ROOT_DIR}/include")
set(LIB_FLAGS -LC:/C/SDL2/lib -lmingw32 -lrpcrt4 -lSDL2main -lSDL2.dll -lSDL2_image -lSDL2_ttf -lm)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unknown-pragmas -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "C:/C/SDL2/lib/cmake/SDL2")

if (WIN32)
    set(SDL2 "C:/C/SDL2")
    set(RPCRT4 "C:/C/Rpcrt4")

    set(HEADER_DIRS ${HEADER_DIRS} ${SDL2}/include)
    set(HEADER_DIRS ${HEADER_DIRS} ${RPCRT4}/include)
    set(PLATFORM_LIB_FLAGS ${LIB_FLAGS}
            ${SDL2}/lib/libSDL2main.a
            ${SDL2}/lib/libSDL2.dll.a
            ${SDL2}/lib/libSDL2_image.a
            ${SDL2}/lib/libSDL2_ttf.a
            ${RPCRT4}/lib/librpcrt4.a)

    message(STATUS "Building for Windows")

elseif (UNIX AND NOT APPLE)
    set(HEADER_DIRS ${HEADER_DIRS} "/usr/include/SDL2")
    set(PLATFORM_LIB_FLAGS ${LIB_FLAGS})

    message(STATUS "Building for Linux")

endif ()

include_directories(${HEADER_DIRS})

# Main

set(EXECUTABLE_OUTPUT_PATH "${ROOT_DIR}/bin")
add_executable(main ${SRC_DIR}/Main/Main.cpp)
target_sources(main PRIVATE
        ${SRC_DIR}/Utils/Utils.cpp
        ${SRC_DIR}/WindowRenderer.cpp
        ${SRC_DIR}/Entity/Text.cpp
        ${SRC_DIR}/EntityBase/Entity.cpp
        ${SRC_DIR}/EntityBase/AnimatedEntity.cpp
        ${SRC_DIR}/Keqing.cpp
        ${SRC_DIR}/World/Background.cpp
        ${SRC_DIR}/Entity/Particle.cpp
        ${SRC_DIR}/World/World.cpp
        ${SRC_DIR}/World/Block.cpp
        ${SRC_DIR}/Utils/Global.cpp
        ${SRC_DIR}/UI/Button.cpp
        ${SRC_DIR}/EntityBase/WorldEntity.cpp
        ${SRC_DIR}/Main/HomeMenu.cpp
        ${SRC_DIR}/Main/Game1.cpp
        ${SRC_DIR}/Main/ColorKeqing.cpp
        ${SRC_DIR}/Utils/Events.cpp
        ${SRC_DIR}/UI/ColorPicker.cpp
        ${SRC_DIR}/Utils/SpriteArray.cpp)
target_link_libraries(main ${PLATFORM_LIB_FLAGS})
