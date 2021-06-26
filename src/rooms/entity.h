#pragma once
#include <SDL.h>


class Entity
{
public:
    Entity(SDL_Renderer* rend, SDL_Point pos, SDL_Texture* atlas, SDL_Point atlas_pos);

    void render();

    void move(int x, int y);

private:
    SDL_Rect m_rect;
    SDL_Rect m_atlas_rect;
    SDL_Texture* m_atlas;

    SDL_Renderer* m_rend;
};

