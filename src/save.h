#pragma once
#include "common.h"
#include <SDL.h>


class Save
{
public:
    Save(SDL_Renderer* rend, SDL_Texture* atlas, Sprite sprite);

    void render();
    
    void move(int x, int y);

    SDL_Rect rect() { return m_sprite.dst; }

private:
    SDL_Renderer* m_rend;
    Sprite m_sprite;
    SDL_Texture* m_atlas;
};
