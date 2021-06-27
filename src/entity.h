#pragma once
#include <vector>
#include <string>
#include <memory>
#include <SDL.h>


class Entity
{
public:
    Entity(SDL_Renderer* rend, SDL_Point pos, SDL_Texture* atlas, SDL_Point atlas_pos, const std::vector<std::string>& dialogue, const std::string& theme_path);

    void render();

    void move(int x, int y);

    SDL_Rect rect() { return m_rect; }
    std::vector<std::string> dialogue() { return m_dialogue; }
    std::string theme() { return m_theme_path; }

private:
    SDL_Rect m_rect;
    SDL_Rect m_atlas_rect;
    SDL_Texture* m_atlas;

    SDL_Renderer* m_rend;

    std::vector<std::string> m_dialogue;
    std::vector<std::string> m_battle_dialogue;

    std::string m_theme_path;
};

