cmake_minimum_required(VERSION 3.20.4)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(undersus)

find_package(jsoncpp CONFIG REQUIRED)

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
    src/save.h
    src/save.cpp
)

target_link_libraries(undersus PRIVATE graphics audio jsoncpp_object jsoncpp_static)

if (MSVC)
    set_property(TARGET undersus PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()

