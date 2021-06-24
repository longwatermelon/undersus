project(gui)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)

add_library(gui
    src/text.h
    src/text.cpp
    src/common.h
    src/common.cpp
    src/image.h
    src/image.cpp
    src/menu.h
    src/menu.cpp
)

target_link_libraries(gui PUBLIC 
    SDL2::SDL2 SDL2::SDL2main
    SDL2::SDL2_image
    SDL2::SDL2_ttf
    SDL2::SDL2_mixer
)

target_include_directories(undersus PRIVATE src)

