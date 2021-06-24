#pragma once
#include <string>
#include <SDL.h>


class Player
{
public:
    Player(SDL_Renderer* rend, SDL_Rect rect, const std::string& sprite_path);

    void render();

private:
    SDL_Rect m_rect;
    SDL_Texture* m_tex{ 0 };

    SDL_Renderer* m_rend;
};
