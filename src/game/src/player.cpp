#include "player.h"
#include <iostream>
#include <algorithm>
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


void Player::move(const std::string& layout, int characters_per_line, const std::vector<char>& solid_characters)
{
    int xo = (m_velocity.x > 0 ? m_rect.w : 0);
    int yo = (m_velocity.y > 0 ? m_rect.h : 0);

    int current_y = (int)(m_rect.y / (800 / characters_per_line));
    int current_x = (int)(m_rect.x / (800 / characters_per_line));

    int new_y = (int)((m_rect.y + m_velocity.y + yo) / (800 / characters_per_line));
    int new_x = (int)((m_rect.x + m_velocity.x + xo) / (800 / characters_per_line));

    if (std::find(solid_characters.begin(), solid_characters.end(), layout[current_y * characters_per_line + new_x]) == solid_characters.end())
    {
        m_rect.x += m_velocity.x;
    }
    else // inside of a solid block
    {
        m_rect.x += (new_x * (800 / characters_per_line) + (xo == 0 ? 32 : 0 )) - (m_rect.x + xo);
    }
    
    if (std::find(solid_characters.begin(), solid_characters.end(), layout[new_y * characters_per_line + current_x]) == solid_characters.end())
    {
        m_rect.y += m_velocity.y;
    }
    else // inside of a solid block
    {
        m_rect.y += (new_y * (800 / characters_per_line) + (yo == 0 ? 32 : 0)) - (m_rect.y + yo);
    }
}

