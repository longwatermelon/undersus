#include "battle.h"
#include "audio/src/audio.h"
#include <iostream>


Battle::Battle(SDL_Renderer* rend, Entity* ent, SDL_Texture* atlas, const std::string& resources_dir)
    : m_rend(rend), m_entity(ent), m_atlas(atlas), m_resources_dir(resources_dir)
{
    audio::play_music(m_entity->theme());
}


Battle::~Battle()
{
    audio::stop_music();
}


void Battle::render()
{
    SDL_Rect rect = { 0, 0, 800, 800 };
    SDL_RenderFillRect(m_rend, &rect);

    m_entity->render();

    SDL_Rect src = { 32, 32, 32, 32 };
    SDL_Rect dst = { 100, 600, 64, 64 };

    if (m_current_selected_button == 0 && m_turn == Turn::PLAYER)
    {
        src.x -= 32;
        src.y += 32;
    }

    SDL_RenderCopy(m_rend, m_atlas, &src, &dst);

    src = { 64, 32, 32, 32 };
    dst = { 500, 600, 64, 64 };

    if (m_current_selected_button == 1 && m_turn == Turn::PLAYER)
    {
        src.x -= 32;
        src.y += 32;
    }

    SDL_RenderCopy(m_rend, m_atlas, &src, &dst);
}


void Battle::move_selected(int x)
{
    m_current_selected_button = std::min(std::max(m_current_selected_button + x, 0), 1);
}


void Battle::hit_selected_button()
{
    if (m_turn == Turn::PLAYER)
    {
        switch (m_current_selected_button)
        {
        case 0: // fight
            audio::play_sound(m_resources_dir + "sfx/kill.wav");
            m_entity->die();
            m_finished = true;
            break;
        case 1: // spare
            m_turn = Turn::ENEMY;
            break;
        }
    }
}

