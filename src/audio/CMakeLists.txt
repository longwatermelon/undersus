project(audio)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-mixer CONFIG REQUIRED)

add_library(audio
    src/audio.h
    src/audio.cpp
)

target_link_libraries(audio PUBLIC
    SDL2::SDL2 SDL2::SDL2main
    SDL2::SDL2_mixer
)

