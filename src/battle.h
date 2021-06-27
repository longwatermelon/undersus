#pragma once
#include "entity.h"


class Battle
{
public:
    Battle(SDL_Renderer* rend, Entity* ent);

    void render();

private:
    SDL_Renderer* m_rend;
    Entity* m_entity;
};

