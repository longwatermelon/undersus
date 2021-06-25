#pragma once
#include "room.h"
#include <string>
#include <vector>
#include <chrono>
#include <SDL.h>


class Player
{
public:
    Player(SDL_Renderer* rend, SDL_Rect rect, const std::string& sprite_path);
    ~Player();

    void render();

    void move(Room* room, const std::vector<char>& solid_characters);

    void animate();

    void set_x_vel(int x) { m_velocity.x = x; }
    void set_y_vel(int y) { m_velocity.y = y; }

    SDL_Rect rect() { return m_rect; }

    void move_to(int x, int y) { m_rect = { x, y, m_rect.w, m_rect.h }; }

private:
    SDL_Rect m_rect;

    SDL_Renderer* m_rend;

    SDL_Point m_velocity{ 0, 0 };

    std::vector<SDL_Texture*> m_animation_frames;
    int m_current_frame{ 0 };

    std::chrono::system_clock::time_point m_last_frame_change;
};
