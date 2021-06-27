#include "entity.h"
#include "common.h"


Entity::Entity(SDL_Renderer* rend, SDL_Point pos, SDL_Texture* atlas, SDL_Point atlas_pos, const std::vector<std::string>& dialogue)
    : m_rend(rend), m_rect{ pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE }, m_atlas(atlas), m_atlas_rect{ atlas_pos.x, atlas_pos.y, BLOCK_SIZE, BLOCK_SIZE }, m_dialogue(dialogue) {}


void Entity::render()
{
    SDL_RenderCopy(m_rend, m_atlas, &m_atlas_rect, &m_rect);
}


void Entity::move(int x, int y)
{
    m_rect.x += x;
    m_rect.y += y;
}

