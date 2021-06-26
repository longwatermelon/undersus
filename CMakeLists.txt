cmake_minimum_required(VERSION 3.20.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(undersus)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set_property(TARGET undersus PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)

add_executable(undersus
    src/main.cpp
    src/game.h
    src/game.cpp
    src/player.h
    src/player.cpp

    src/rooms/room.h
    src/rooms/room.cpp
    src/rooms/common.h
    src/rooms/entity.h
    src/rooms/entity.cpp

    src/graphics/common.h
    src/graphics/common.cpp
    src/graphics/image.h
    src/graphics/image.cpp
    src/graphics/menu.h
    src/graphics/menu.cpp
    src/graphics/text.h
    src/graphics/text.cpp
    src/graphics/textbox.h
    src/graphics/textbox.cpp

    src/audio/audio.h
    src/audio/audio.cpp
)

target_link_libraries(undersus PRIVATE
    SDL2::SDL2 SDL2::SDL2main
    SDL2::SDL2_image
    SDL2::SDL2_ttf
    SDL2::SDL2_mixer
)

