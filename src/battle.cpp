#include "battle.h"
#include "audio/src/audio.h"
#include "game.h"
#include <iostream>


Battle::Battle(SDL_Renderer* rend, Entity* ent, SDL_Texture* atlas, const std::string& resources_dir)
    : m_rend(rend), m_entity(ent), m_atlas(atlas), m_resources_dir(resources_dir)
{
    audio::play_music(m_entity->theme());
    m_player.src = { 0, 96, 32, 32 };
    m_player.dst = { 384, 384, 32, 32 };
}


Battle::~Battle()
{
    audio::stop_music();
}


void Battle::render()
{
    SDL_SetRenderDrawColor(m_rend, 0, 0, 0, 255);

    SDL_Rect rect = { 0, 0, 800, 800 };
    SDL_RenderFillRect(m_rend, &rect);

    SDL_SetRenderDrawColor(m_rend, 255, 255, 255, 255);
    SDL_RenderFillRect(m_rend, &m_box);

    SDL_SetRenderDrawColor(m_rend, 0, 0, 0, 255);
    rect = { m_box.x + 5, m_box.y + 5, m_box.w - 10, m_box.h - 10 };
    SDL_RenderFillRect(m_rend, &rect);
    
    m_entity->render();

    SDL_Rect src = { 32, 32, 32, 32 };
    SDL_Rect dst = { 180, 700, 64, 64 };

    if (m_current_selected_button == 0 && m_turn == Turn::PLAYER)
    {
        src.x -= 32;
        src.y += 32;
    }

    SDL_RenderCopy(m_rend, m_atlas, &src, &dst);

    src = { 64, 32, 32, 32 };
    dst = { 560, 700, 64, 64 };

    if (m_current_selected_button == 1 && m_turn == Turn::PLAYER)
    {
        src.x -= 32;
        src.y += 32;
    }

    SDL_RenderCopy(m_rend, m_atlas, &src, &dst);

    SDL_RenderCopy(m_rend, m_atlas, &m_player.src, &m_player.dst);

    for (auto& p : m_projectiles)
    {
        SDL_RenderCopy(m_rend, m_atlas, &p.sprite.src, &p.sprite.dst);
    }

    if (std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_attack_start).count() > m_entity->attacks()[m_current_attack_index].second)
    {
        m_turn = Turn::PLAYER;
        m_projectiles.clear();
    }
}


void Battle::move_projectiles()
{
    for (int i = 0; i < m_projectiles.size(); ++i)
    {
        auto& p = m_projectiles[i];

        p.sprite.dst.x += p.vector.x;
        p.sprite.dst.y += p.vector.y;
    }
}


void Battle::check_collisions()
{
    SDL_Rect pr = m_player.dst;

    for (auto& p : m_projectiles)
    {
        SDL_Rect br = p.sprite.dst;

        if (pr.x <= br.x + br.w && pr.x + pr.w >= br.x &&
            pr.y <= br.y + br.h && pr.y + pr.h >= br.y)
        {
            audio::play_sound(m_resources_dir + "sfx/kill.wav");
            m_player_dead = true;
            m_finished = true;
        }
    }
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

        if (!m_finished)
        {
            start_attacks();
        }
    }
}


void Battle::add_projectile(Projectile p)
{
    m_projectiles.emplace_back(p);
}


void Battle::start_attacks()
{
    m_current_attack_index = randint(0, m_entity->attacks().size() - 1);
    m_entity->attacks()[m_current_attack_index].first();
    m_attack_start = std::chrono::system_clock::now();
}


void Battle::move_player()
{
    if (m_turn == Turn::ENEMY)
    {
        m_player.dst.x += m_player_vector.x;
        m_player.dst.y += m_player_vector.y;
    }
}

