#pragma once
#include <string>
#include <vector>
#include <SDL.h>


class Player
{
public:
    Player(SDL_Renderer* rend, SDL_Rect rect, const std::string& sprite_path);

    void render();

    void move(const std::string& layout, int characters_per_line, const std::vector<char>& solid_characters);

    void set_x_vel(int x) { m_velocity.x = x; }
    void set_y_vel(int y) { m_velocity.y = y; }

    SDL_Rect rect() { return m_rect; }

    void move_to(int x, int y) { m_rect = { x, y, m_rect.w, m_rect.h }; }

private:
    SDL_Rect m_rect;
    SDL_Texture* m_tex{ 0 };

    SDL_Renderer* m_rend;

    SDL_Point m_velocity{ 0, 0 };
};
