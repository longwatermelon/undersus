#include "common.h"


SDL_Texture* gui::common::render_text(SDL_Renderer* rend, const Font& font, const std::string& text, SDL_Color color)
{
    if (text.empty())
        return 0;

    SDL_Surface* surf = TTF_RenderText_Blended(font.font, text.c_str(), color);
    SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);

    SDL_FreeSurface(surf);

    return tex;
}

