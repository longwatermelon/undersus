#pragma once
#include "text.h"
#include "image.h"
#include <string>
#include <memory>
#include <vector>
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
    std::string m_font_path;

    bool m_running{ true };

    std::vector<std::unique_ptr<gui::Text>> m_text;
    std::vector<std::unique_ptr<gui::Image>> m_images;
};
