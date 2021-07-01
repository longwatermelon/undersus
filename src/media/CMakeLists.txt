project(media)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)

add_library(media
    src/audio.h
    src/audio.cpp
    src/common.h
    src/common.cpp
    src/image.h
    src/image.cpp
    src/menu.h
    src/menu.cpp
    src/text.h
    src/text.cpp
    src/textbox.h
    src/textbox.cpp
)

target_link_libraries(media PUBLIC
    SDL2::SDL2 SDL2::SDL2main
    SDL2::SDL2_ttf
    SDL2::SDL2_image
    SDL2::SDL2_mixer
)

