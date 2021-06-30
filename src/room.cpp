#include "room.h"
#include "common.h"
#include <iostream>


Room::Room(SDL_Renderer* rend, const std::string& layout, int characters_per_row, const std::map<char, SDL_Point>& textures, SDL_Texture* atlas, SDL_Point left_start_pos, SDL_Point right_start_pos, SDL_Point render_pos)
    : m_layout(layout), m_characters_per_row(characters_per_row), m_textures(textures), m_texture_atlas(atlas), m_rend(rend), m_left_start_pos(left_start_pos), m_right_start_pos(right_start_pos), m_render_pos(render_pos) {}


void Room::render()
{
    int current_y = m_render_pos.y;

    for (int i = 0; i < m_layout.size(); ++i)
    {
        if (i % m_characters_per_row == 0 && i != 0)
            current_y += BLOCK_SIZE;

        if (m_textures.find(m_layout[i]) == m_textures.end())
            continue;

        SDL_Rect dst = {
            // take remainder of i / m_characters_per_row instead of using i because x should not be affected by the y position of the texture
            m_render_pos.x + (i % m_characters_per_row) * BLOCK_SIZE,
            current_y,
            BLOCK_SIZE,
            BLOCK_SIZE
        };

        SDL_Rect src = {
            m_textures[m_layout[i]].x,
            m_textures[m_layout[i]].y,
            BLOCK_SIZE,
            BLOCK_SIZE
        };

        SDL_RenderCopy(m_rend, m_texture_atlas, &src, &dst);
    }

    for (auto& entity : m_data->entities)
    {
        entity->render();
    }
}


void Room::move(int x, int y)
{
    m_render_pos.x += x;
    m_render_pos.y += y;

    for (auto& entity : m_data->entities)
    {
        entity->move(x, y);
    }
}


bool Room::moveable(int x, int y)
{
    int pixel_width = m_characters_per_row * BLOCK_SIZE;
   
    if ((x < 0 && pixel_width + m_render_pos.x > 800) || (x > 0 && m_render_pos.x < 0))
    {
        return true; 
    }

    int pixel_height = (m_layout.size() / m_characters_per_row) * BLOCK_SIZE;
    
    if ((y < 0 && pixel_height + m_render_pos.y > 800) || (y > 0 && m_render_pos.y < 0))
    {
        return true;
    }

    return false;
}


void Room::add_data(std::unique_ptr<RoomData> data)
{
    m_data = std::move(data);
}
