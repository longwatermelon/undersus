#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <SDL.h>


class Entity
{
public:
    Entity(SDL_Renderer* rend, SDL_Point pos, SDL_Texture* atlas, SDL_Point alive_spr, SDL_Point dead_spr, const std::string& theme_path, const std::vector<std::string>& dialogue, const std::vector<std::pair<std::function<void(void)>, int>>& attacks);

    void render();

    void move(int x, int y);


    void die() { m_alive = false; }

    SDL_Rect rect() { return m_rect; }
    std::vector<std::string> dialogue() { return m_dialogue; }
    std::string theme() { return m_theme_path; }
    bool alive() { return m_alive; }
    std::vector<std::pair<std::function<void(void)>, int>>& attacks() { return m_attacks; }

private:
    SDL_Rect m_rect;
    SDL_Rect m_sprite;
    SDL_Rect m_dead_sprite;
    SDL_Texture* m_atlas;

    SDL_Renderer* m_rend;

    std::vector<std::string> m_dialogue;
    std::vector<std::string> m_battle_dialogue;

    std::string m_theme_path;

    std::vector<std::pair<std::function<void(void)>, int>> m_attacks;

    bool m_alive{ true };
};

