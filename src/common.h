#pragma once
#include <random>
#include <iostream>
#include <SDL.h>

inline constexpr int BLOCK_SIZE = 32;

struct Sprite
{
    SDL_Rect src;
    SDL_Rect dst;
};

enum class Route
{
    PACIFIST,
    GENOCIDE
};

inline int randint(int min, int max)
{
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(min, max);

    return uni(rng);
}

struct TextureDeleter
{
    void operator()(SDL_Texture* tex)
    {
        if (tex)
            SDL_DestroyTexture(tex);

        tex = 0;
    }
};


inline std::vector<std::string> split_string(std::string string, char delim)
{
    std::vector<std::string> ret;

    for (int i = 0; i < string.size(); ++i)
    {
        if (string[i] == ' ')
        {
            ret.emplace_back(string.substr(0, i));
            string.erase(0, i + 1);
            i = 0;
        }
    }

    if (!string.empty())
    {
        ret.emplace_back(string);
    }
    
    return ret;
}

