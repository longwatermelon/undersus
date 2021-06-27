#pragma once
#include "graphics/src/image.h"
#include "entity.h"
#include <memory>


class Battle
{
public:
    Battle(SDL_Renderer* rend, Entity* ent, SDL_Texture* atlas);
    ~Battle();

    void render();

    void move_selected(int x);
    void hit_selected_button();

private:
    SDL_Renderer* m_rend;
    Entity* m_entity;

    SDL_Texture* m_atlas;

    int m_current_selected_button{ 0 };
};

