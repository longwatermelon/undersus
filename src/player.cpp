#include "player.h"
#include "rooms/common.h"
#include "graphics/common.h"
#include <iostream>
#include <algorithm>
#include <SDL_image.h>


Player::Player(SDL_Renderer* rend, SDL_Rect rect, const std::string& sprite_path)
    : m_rect(rect), m_rend(rend)
{
    m_atlas = IMG_LoadTexture(rend, sprite_path.c_str());
    m_last_frame_change = std::chrono::system_clock::now();

    m_animation_frames["rwalk_0"] = { 64, 0 };
    m_animation_frames["rwalk_1"] = { 0, 0 };
    m_animation_frames["rwalk_2"] = { 32, 0 };
    m_animation_frames["rwalk_3"] = { 0, 0 };

    m_animation_frames["lwalk_0"] = { 32, 32 };
    m_animation_frames["lwalk_1"] = { 64, 32 };
    m_animation_frames["lwalk_2"] = { 0, 32 };
    m_animation_frames["lwalk_3"] = { 64, 32 };
}


Player::~Player()
{
    SDL_DestroyTexture(m_atlas);
}


void Player::render()
{
    SDL_Rect rect = {
        m_current_frame_pos.x,
        m_current_frame_pos.y,
        BLOCK_SIZE,
        BLOCK_SIZE 
    };

    SDL_RenderCopy(m_rend, m_atlas, &rect, &m_rect);
}


void Player::move(Room* room, const std::vector<char>& solid_characters)
{
    if (!m_moveable)
        return;

    // avoid using ternary operator because when player isnt moving direction shouldnt be changed
    if (m_velocity.x > 0)
        m_direction = 1;

    if (m_velocity.x < 0)
        m_direction = -1;

    int characters_per_line = room->characters_per_line();
    std::string layout = room->layout();

    int xo = (m_velocity.x > 0 ? m_rect.w : 0);
    int yo = (m_velocity.y > 0 ? m_rect.h : 0);

    int px = m_rect.x - room->render_pos().x;
    int py = m_rect.y - room->render_pos().y;

    int current_y = (int)(py / BLOCK_SIZE);
    int current_x = (int)(px / BLOCK_SIZE);

    int new_y = (int)((py + m_velocity.y + yo) / BLOCK_SIZE);
    int new_x = (int)((px + m_velocity.x + xo) / BLOCK_SIZE);

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
        }
        
    }
    
    if (!gui::common::exists(layout[new_y * characters_per_line + (current_x + 1)], solid_characters))
    {
        if (((m_rect.y >= 600 && m_velocity.y > 0) || (m_rect.y <= 200 && m_velocity.y < 0)) && room->moveable(0, -m_velocity.y))
        {
            room->move(0, -m_velocity.y);
        }
        else
        {
            if (!gui::common::exists(layout[new_y * characters_per_line + current_x], solid_characters))
            {
                m_rect.y += m_velocity.y;
            }
        }
    }
}


void Player::animate()
{
    if (std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_last_frame_change).count() >= 110)
    {
        m_last_frame_change = std::chrono::system_clock::now();

        if ((m_velocity.x != 0 || m_velocity.y != 0) && m_moveable) // moving
        {
            if (m_current_frame_num >= 4)
            {
                m_current_frame_num = 0;
            }

            m_current_frame_pos = m_animation_frames[std::string(m_direction == 1 ? "r" : "l") + "walk_" + std::to_string(m_current_frame_num)];
            ++m_current_frame_num;
        }
        else // idle
        {
            m_current_frame_num = 0;
            m_current_frame_pos = m_animation_frames[std::string(m_direction == 1 ? "r" : "l") + "walk_1"];
        }
    }
}


char Player::current_character(Room* room)
{
    int px = m_rect.x - room->render_pos().x;
    int py = m_rect.y - room->render_pos().y;

    int current_y = (int)(py / BLOCK_SIZE);
    int current_x = (int)(px / BLOCK_SIZE);

    return room->layout()[current_y * room->characters_per_line() + current_x];
}

