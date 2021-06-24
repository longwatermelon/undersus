project(undersus)

add_executable(undersus
    src/main.cpp
    src/game.h
    src/game.cpp
)

target_link_libraries(undersus PRIVATE gui)

