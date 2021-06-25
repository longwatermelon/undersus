#include "room.h"


Room::Room(SDL_Renderer* rend, const std::string& layout, int characters_per_row, const std::map<char, SDL_Point>& textures, SDL_Texture* atlas, SDL_Point left_start_pos, SDL_Point right_start_pos)
    : m_layout(layout), m_characters_per_row(characters_per_row), m_textures(textures), m_texture_atlas(atlas), m_rend(rend), m_left_start_pos(left_start_pos), m_right_start_pos(right_start_pos) {}


void Room::render()
{
    // initially set to -32 because 0 % characters_per_row is 0, adding 32 to current_y
    int current_y = -32;

    for (int i = 0; i < m_layout.size(); ++i)
    {
        if (i % m_characters_per_row == 0)
            current_y += 32;

        SDL_Rect dst = {
            // take remainder of i / m_characters_per_row instead of using i because x should not be affected by the y position of the texture
            (i % m_characters_per_row) * 32,
            current_y,
            32,
            32
        };

        SDL_Rect src = {
            m_textures[m_layout[i]].x,
            m_textures[m_layout[i]].y,
            32,
            32
        };

        SDL_RenderCopy(m_rend, m_texture_atlas, &src, &dst);
    }
}

