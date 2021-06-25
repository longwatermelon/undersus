#pragma once
#include <string>
#include <SDL.h>


class Player
{
public:
    Player(SDL_Renderer* rend, SDL_Rect rect, const std::string& sprite_path);

    void render();

    void move();

    void set_x_vel(int x) { m_velocity.x = x; }
    void set_y_vel(int y) { m_velocity.y = y; }

private:
    SDL_Rect m_rect;
    SDL_Texture* m_tex{ 0 };

    SDL_Renderer* m_rend;

    SDL_Point m_velocity{ 0, 0 };
};
