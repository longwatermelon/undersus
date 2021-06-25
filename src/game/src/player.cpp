#include "player.h"
#include <iostream>
#include <SDL_image.h>


Player::Player(SDL_Renderer* rend, SDL_Rect rect, const std::string& sprite_path)
    : m_rect(rect), m_rend(rend)
{
    m_tex = IMG_LoadTexture(rend, sprite_path.c_str());
}


void Player::render()
{
    SDL_RenderCopy(m_rend, m_tex, 0, &m_rect);
}


void Player::move()
{
    m_rect.x += m_velocity.x;
    m_rect.y += m_velocity.y;
}

