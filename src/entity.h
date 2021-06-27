#pragma once
#include <vector>
#include <string>
#include <SDL.h>


class Entity
{
public:
    Entity(SDL_Renderer* rend, SDL_Point pos, SDL_Texture* atlas, SDL_Point atlas_pos, const std::vector<std::string>& dialogue);

    void render();

    void move(int x, int y);

    SDL_Rect rect() { return m_rect; }
    std::vector<std::string> dialogue() { return m_dialogue; }

private:
    SDL_Rect m_rect;
    SDL_Rect m_atlas_rect;
    SDL_Texture* m_atlas;

    SDL_Renderer* m_rend;

    std::vector<std::string> m_dialogue;
    std::vector<std::string> m_battle_dialogue;
};

