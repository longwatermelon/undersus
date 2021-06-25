#include "player.h"
#include "graphics/common.h"
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


void Player::move(Room* room, const std::vector<char>& solid_characters)
{
    int characters_per_line = room->characters_per_line();
    std::string layout = room->layout();

    int xo = (m_velocity.x > 0 ? m_rect.w : 0);
    int yo = (m_velocity.y > 0 ? m_rect.h : 0);

    int tile_size = 32;

    int px = m_rect.x - room->render_pos().x;
    int py = m_rect.y - room->render_pos().y;

    int current_y = (int)(py / tile_size);
    int current_x = (int)(px / tile_size);

    int new_y = (int)((py + m_velocity.y + yo) / tile_size);
    int new_x = (int)((px + m_velocity.x + xo) / tile_size);

    if (!gui::common::exists(layout[(current_y + 1) * characters_per_line + new_x], solid_characters))
    {
        // start scrolling the screen
        if (((m_rect.x >= 600 && m_velocity.x > 0) || (m_rect.x <= 200 && m_velocity.x < 0)) && room->moveable(-m_velocity.x, 0))
        {
            room->move(-m_velocity.x, 0);
        }
        else // screen is not scrollable
        {
            if (!gui::common::exists(layout[current_y * characters_per_line + new_x], solid_characters))
            {
                m_rect.x += m_velocity.x;
            }
            else // inside of a solid block
            {
                m_rect.x += (new_x * tile_size + (xo == 0 ? tile_size : 0 )) - (px + xo);
            }
        }
        
    }
    
    
    if (!gui::common::exists(layout[new_y * characters_per_line + (current_x + 1)], solid_characters))
    {
        if (!gui::common::exists(layout[new_y * characters_per_line + current_x], solid_characters))
        {
            m_rect.y += m_velocity.y;
        }
        else // inside of a solid block
        {
            m_rect.y += (new_y * tile_size + (yo == 0 ? tile_size : 0)) - (m_rect.y + yo);
        }
    }
}

