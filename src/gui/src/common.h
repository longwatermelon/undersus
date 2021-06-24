#pragma once
#include <string>
#include <SDL_ttf.h>


namespace gui::common
{
    struct Font
    {
        TTF_Font* font;
        int ptsize;
    };


    SDL_Texture* render_text(SDL_Renderer* rend, const Font& font, const std::string& text, SDL_Color color);
}
