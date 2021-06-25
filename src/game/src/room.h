#pragma once
#include <string>
#include <map>
#include <SDL.h>


class Room
{
public:
    Room(SDL_Renderer* rend, const std::string& layout, int characters_per_row, const std::map<char, SDL_Point>& textures, SDL_Texture* atlas);

    void render();

private:
    std::string m_layout;
    int m_characters_per_row;
    std::map<char, SDL_Point> m_textures;

    SDL_Texture* m_texture_atlas;

    SDL_Renderer* m_rend;
};