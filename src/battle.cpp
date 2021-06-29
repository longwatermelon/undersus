#include "battle.h"
#include "audio/src/audio.h"
#include "game.h"
#include "graphics/src/textbox.h"
#include <iostream>


Battle::Battle(SDL_Renderer* rend, Entity* ent, SDL_Texture* atlas, const std::string& resources_dir)
    : m_rend(rend), m_entity(ent), m_atlas(atlas), m_resources_dir(resources_dir)
{
    audio::play_music(m_entity->theme());
    m_player.src = { 0, 96, 32, 32 };
    m_player.dst = { 384, 384, 16, 16 };

    m_entity_spr.src = ent->battle_sprite();
    m_entity_spr.dst = { 368, 100, 64, 64 };

    m_turn = Turn::ENEMY;
    m_z_down = false;
    int index = randint(0, m_entity->battle_dialogue().size() - 1);

    m_current_textbox = std::unique_ptr<gui::Textbox>(new gui::Textbox(m_rend, { 500, 40, 200, 120 }, m_entity->battle_dialogue()[index], m_resources_dir + "gfx/font.ttf", 12, false, { 255, 255, 255 }, { 0, 0, 0 }));
    m_choice_text = std::unique_ptr<gui::Text>(new gui::Text(m_rend, { 360, 700 }, "FIGHT", m_resources_dir + "gfx/font.ttf", 16, { 255, 255, 255 }, -1));
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

    SDL_RenderCopy(m_rend, m_atlas, &m_entity_spr.src, &m_entity_spr.dst);

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

    if (m_turn == Turn::ENEMY)
        SDL_RenderCopy(m_rend, m_atlas, &m_player.src, &m_player.dst);

    if (m_current_textbox)
        m_current_textbox->render();

    if (m_choice_text && m_turn == Turn::PLAYER)
        m_choice_text->render();

    if (m_z_down)
    {
        if (m_current_textbox)
            m_current_textbox.reset(0);

        m_z_down = false;
    }

    for (auto& p : m_projectiles)
    {
        if (std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_attack_start).count() < p.wait_time)
            continue;

        SDL_RenderCopy(m_rend, m_atlas, &p.sprite.src, &p.sprite.dst);
    }

    if (m_turn == Turn::ENEMY && !m_current_textbox && !m_attacking && !m_finished)
    {
        start_attacks();
    }

    if (std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_attack_start).count() > m_entity->attacks()[m_current_attack_index].second && !m_current_textbox)
    {
        m_attacking = false;
        m_turn = Turn::PLAYER;
        m_projectiles.clear();
    }
}


void Battle::move_projectiles()
{
    for (int i = 0; i < m_projectiles.size(); ++i)
    {
        auto& p = m_projectiles[i];

        if (std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_attack_start).count() < p.wait_time)
            continue;

        p.sprite.dst.x += p.vector.x;
        p.sprite.dst.y += p.vector.y;
    }
}


void Battle::check_collisions()
{
    SDL_Rect pr = { m_player.dst.x + 5, m_player.dst.y + 5, m_player.dst.w - 10, m_player.dst.h - 10 };

    for (auto& p : m_projectiles)
    {
        SDL_Rect br = p.sprite.dst;

        if (std::chrono::duration<float, std::milli>(std::chrono::system_clock::now() - m_attack_start).count() < p.wait_time)
            continue;


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
    if (m_turn == Turn::PLAYER)
        m_current_selected_button = std::min(std::max(m_current_selected_button + x, 0), 1);

    if (m_current_selected_button == 0)
    {
        m_choice_text = std::unique_ptr<gui::Text>(new gui::Text(m_rend, { 360, 700 }, "FIGHT", m_resources_dir + "gfx/font.ttf", 16, { 255, 255, 255 }, -1));
    }
    else
    {
        m_choice_text = std::unique_ptr<gui::Text>(new gui::Text(m_rend, { 360, 700 }, "SPARE", m_resources_dir + "gfx/font.ttf", 16, { 255, 255, 255 }, -1));
    }
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
            m_player.dst.x = 384;
            m_player.dst.y = 384;

            m_z_down = false;
            int index = randint(0, m_entity->battle_dialogue().size() - 1);

            m_current_textbox = std::unique_ptr<gui::Textbox>(new gui::Textbox(m_rend, { 500, 40, 200, 120 }, m_entity->battle_dialogue()[index], m_resources_dir + "gfx/font.ttf", 12, false, { 255, 255, 255 }, { 0, 0, 0 }));
        }
    }
}


void Battle::add_projectile(Projectile p)
{
    m_projectiles.emplace_back(p);
}


void Battle::start_attacks()
{
    m_attacking = true;
    m_attack_start = std::chrono::system_clock::now();
    m_current_attack_index = randint(0, m_entity->attacks().size() - 1);
    m_entity->attacks()[m_current_attack_index].first();
    m_attack_start = std::chrono::system_clock::now();
}


void Battle::move_player()
{
    if (m_turn == Turn::ENEMY && !m_current_textbox)
    {
        if (m_player.dst.x + m_player_vector.x < m_box.x + m_box.w - m_player.dst.w && m_player.dst.x + m_player_vector.x > m_box.x)
            m_player.dst.x += m_player_vector.x;

        if (m_player.dst.y + m_player_vector.y < m_box.y + m_box.h - m_player.dst.h && m_player.dst.y + m_player_vector.y > m_box.y)
            m_player.dst.y += m_player_vector.y;
    }
}

