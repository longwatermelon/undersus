project(undersus)

add_executable(undersus
    src/main.cpp
    src/game.h
    src/game.cpp
    src/player.h
    src/player.cpp
)

target_link_libraries(undersus PRIVATE gui)

