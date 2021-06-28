#include "entity.h"
#include "common.h"


Entity::Entity(SDL_Renderer* rend, SDL_Point pos, SDL_Texture* atlas, SDL_Point alive_spr, SDL_Point dead_spr, const std::string& theme_path, const std::vector<std::string>& dialogue, const std::vector<std::pair<std::function<void(void)>, int>>& attacks)
    : m_rend(rend), m_rect{ pos.x, pos.y, BLOCK_SIZE, BLOCK_SIZE }, m_atlas(atlas), m_sprite{ alive_spr.x, alive_spr.y, BLOCK_SIZE, BLOCK_SIZE }, m_dead_sprite{ dead_spr.x, dead_spr.y, BLOCK_SIZE, BLOCK_SIZE }, m_dialogue(dialogue), m_theme_path(theme_path), m_attacks(attacks) {}


void Entity::render()
{
    if (m_alive)
        SDL_RenderCopy(m_rend, m_atlas, &m_sprite, &m_rect);
    else
        SDL_RenderCopy(m_rend, m_atlas, &m_dead_sprite, &m_rect);
}


void Entity::move(int x, int y)
{
    m_rect.x += x;
    m_rect.y += y;
}

