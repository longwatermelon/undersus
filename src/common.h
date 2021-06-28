#pragma once
#include <random>
#include <SDL.h>

inline constexpr int BLOCK_SIZE = 32;

struct Sprite
{
    SDL_Rect src;
    SDL_Rect dst;
};

inline int randint(int min, int max)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min, max);

    return uni(rng);
}
