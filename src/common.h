#pragma once
#include <SDL.h>

inline constexpr int BLOCK_SIZE = 32;

struct Sprite
{
    SDL_Rect src;
    SDL_Rect dst;
};
