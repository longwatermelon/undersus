#pragma once
#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <SDL.h>


class Entity
{
public:
    Entity(SDL_Renderer* rend, SDL_Point pos, SDL_Texture* atlas, SDL_Point alive_spr, SDL_Point dead_spr, SDL_Point battle_spr, const std::string& theme_path, const std::vector<std::string>& dialogue, const std::vector<std::string>& battle_dialogue, const std::vector<std::string>& post_battle_dialogue, const std::vector<std::pair<std::function<void(void)>, int>>& attacks, bool alive);

    void render();

    void move(int x, int y);

    std::vector<std::string> dialogue();

    void die() { m_alive = false; }
    void set_fought(bool b) { m_fought = b; }

    SDL_Rect rect() { return m_rect; }
    std::vector<std::string> battle_dialogue() { return m_battle_dialogue; }
    std::string theme() { return m_theme_path; }

    bool alive() { return m_alive; }
    bool fought() { return m_fought; }

    std::vector<std::pair<std::function<void(void)>, int>>& attacks() { return m_attacks; }
    SDL_Rect battle_sprite() { return m_battle_sprite; }

private:
    SDL_Rect m_rect;
    SDL_Rect m_sprite;
    SDL_Rect m_dead_sprite;
    SDL_Rect m_battle_sprite;
    SDL_Texture* m_atlas;

    SDL_Renderer* m_rend;

    std::vector<std::string> m_dialogue;
    std::vector<std::string> m_battle_dialogue;
    std::vector<std::string> m_post_battle_dialogue;

    std::string m_theme_path;

    std::vector<std::pair<std::function<void(void)>, int>> m_attacks;

    bool m_alive{ true };
    bool m_fought{ false };
};

