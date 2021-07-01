#include "save.h"


Save::Save(SDL_Renderer* rend, SDL_Texture* atlas, Sprite sprite)
    : m_rend(rend), m_atlas(atlas), m_sprite(sprite) {}


void Save::render()
{
    SDL_RenderCopy(m_rend, m_atlas, &m_sprite.src, &m_sprite.dst);
}


void Save::move(int x, int y)
{
    m_sprite.dst.x += x;
    m_sprite.dst.y += y;
}

