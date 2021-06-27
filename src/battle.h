#pragma once
#include "graphics/src/image.h"
#include "entity.h"
#include <memory>


class Battle
{
public:
    Battle(SDL_Renderer* rend, Entity* ent, SDL_Texture* atlas, const std::string& resources_dir);
    ~Battle();

    void render();

    void move_selected(int x);
    void hit_selected_button();

    bool finished() { return m_finished; }

private:
    SDL_Renderer* m_rend;
    Entity* m_entity;

    SDL_Texture* m_atlas;

    int m_current_selected_button{ 0 };

    bool m_finished{ false };

    std::string m_resources_dir;

    enum class Turn
    {
        PLAYER,
        ENEMY
    } m_turn{ Turn::PLAYER }; 

    SDL_Rect m_box{ 200, 200, 400, 400 };
};

