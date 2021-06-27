#include "battle.h"
#include "audio/src/audio.h"


Battle::Battle(SDL_Renderer* rend, Entity* ent)
    : m_rend(rend), m_entity(ent)
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
}

