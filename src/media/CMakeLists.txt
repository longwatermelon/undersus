project(media)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)

add_library(media
    audio.h
    audio.cpp
    common.h
    common.cpp
    image.h
    image.cpp
    menu.h
    menu.cpp
    text.h
    text.cpp
    textbox.h
    textbox.cpp
)

target_link_libraries(media PUBLIC
    SDL2::SDL2 SDL2::SDL2main
    SDL2::SDL2_ttf
    SDL2::SDL2_image
    SDL2::SDL2_mixer
)

