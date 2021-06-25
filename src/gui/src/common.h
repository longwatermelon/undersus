#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <SDL_ttf.h>


namespace gui::common
{
    struct Font
    {
        TTF_Font* font;
        int ptsize;
    };


    SDL_Texture* render_text(SDL_Renderer* rend, const Font& font, const std::string& text, SDL_Color color);

    template <typename T>
    inline bool exists(T elem, const std::vector<T>& vec)
    {
        return std::find(vec.begin(), vec.end(), elem) != vec.end();
    }
}
