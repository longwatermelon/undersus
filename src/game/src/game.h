#pragma once
#include <string>
#include <SDL.h>


class Game
{
public:
    Game(const std::string& resources_path);
    ~Game();

    void mainloop();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_rend;

    std::string m_resources_dir;

    bool m_running{ true };
};
