#pragma once
#include "graphics/src/image.h"
#include "entity.h"
#include "common.h"
#include <memory>
#include <functional>
#include <thread>

class Game;

struct Projectile
{
    Sprite sprite;
    SDL_Point vector;
};


class Battle
{
public:
    Battle(SDL_Renderer* rend, Entity* ent, SDL_Texture* atlas, const std::string& resources_dir);
    ~Battle();

    void render();

    void move_projectiles();
    void check_collisions();

    void move_selected(int x);
    void hit_selected_button();

    void add_projectile(Projectile p);

    void start_attacks();

    void move_player();
    void set_player_vx(int x) { m_player_vector.x = x; }
    void set_player_vy(int y) { m_player_vector.y = y; }

    bool finished() { return m_finished; }
    bool player_dead() { return m_player_dead; }

private:
    SDL_Renderer* m_rend;
    Entity* m_entity;

    SDL_Texture* m_atlas;

    int m_current_selected_button{ 0 };

    bool m_finished{ false };

    std::string m_resources_dir;

    enum class Turn
    {
        PLAYER,
        ENEMY
    } m_turn{ Turn::PLAYER }; 

    SDL_Rect m_box{ 200, 200, 400, 400 };

    Sprite m_player;
    SDL_Point m_player_vector{ 0, 0 };

    std::vector<Projectile> m_projectiles;

    bool m_player_dead{ false };
};

