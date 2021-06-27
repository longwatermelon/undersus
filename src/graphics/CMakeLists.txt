project(graphics)

find_package(sdl2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(sdl2-ttf CONFIG REQUIRED)

add_library(graphics
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

target_link_libraries(graphics PUBLIC
    SDL2::SDL2 SDL2::SDL2main
    SDL2::SDL2_image
    SDL2::SDL2_ttf
)

