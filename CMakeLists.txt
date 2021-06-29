cmake_minimum_required(VERSION 3.20.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(undersus)

add_subdirectory(src/graphics)
add_subdirectory(src/audio)

add_executable(undersus
    src/main.cpp
    src/game.h
    src/game.cpp
    src/player.h
    src/player.cpp
    src/room.h
    src/room.cpp
    src/common.h
    src/entity.h
    src/entity.cpp
    src/battle.h
    src/battle.cpp
)

target_link_libraries(undersus PRIVATE graphics audio)

if (MSVC)
    set_property(TARGET undersus PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

