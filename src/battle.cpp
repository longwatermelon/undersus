#include "battle.h"


Battle::Battle(SDL_Renderer* rend, Entity* ent)
    : m_rend(rend), m_entity(ent) {}


void Battle::render()
{
    SDL_Rect rect = { 0, 0, 800, 800 };
    SDL_RenderFillRect(m_rend, &rect);

    m_entity->render();
}

